#include "FreeRTOS.h" 
#include "task.h"
#include "stm32f4xx.h"
#include "key.h"
#include "usart1.h"
#include "./led/bsp_led.h"   
#include "queue.h"  
#include "semphr.h"
#include <string.h>
#include "event_groups.h"
#include "timers.h"

// 任务函数声明
void vInitHardware(void *pvParameters);    // 系统硬件初始化任务
void vInitTask(void *pvParameters);        // 应用任务创建和初始化任务
void vTask1(void *pvParameters);           // LED控制任务1，响应LED事件组
void vTask2(void *pvParameters);           // LED控制任务2，响应多种事件
void vTask3(void *pvParameters);           // 按键扫描和事件触发任务
void vTaskUART(void *pvParameters);        // 串口数据接收和处理任务
void vTimerCallback(TimerHandle_t xTimer); // 软件定时器回调函数

// 全局变量声明
TaskHandle_t xSuspendableTaskHandle = NULL;  // 用于存储可挂起任务的句柄，便于后续任务管理

// 事件组句柄声明 - 用于任务间同步和通信
EventGroupHandle_t xInitEventGroup = NULL;   // 系统初始化阶段的任务同步事件组
EventGroupHandle_t xUARTandKey = NULL;       // 串口、按键和定时器事件通信组
EventGroupHandle_t xLEDEventGroup = NULL;    // 专用LED控制事件组

// 软件定时器句柄
TimerHandle_t xPeriodicTimer = NULL;         // 周期性软件定时器，用于生成定时事件

// 事件组标志位定义 - 每个位代表一个特定的事件类型
#define HARDWARE_INIT_COMPLETE_BIT (1 << 0)  // 位0：硬件初始化完成标志，用于系统启动同步

#define UART_BIT (1 << 0)   // 位0：串口数据接收完成事件标志
#define KEY_1_BIT (1 << 1)  // 位1：按键1按下事件标志
#define KEY_2_BIT (1 << 2)  // 位2：按键2按下事件标志
#define Timer_BIT (1 << 3)  // 位3：软件定时器超时事件标志

#define LED_BIT (1 << 0)    // 位0：LED控制事件标志，用于触发LED状态切换

/**
 * @brief 主函数 - 系统入口点，负责基础初始化和启动调度器
 * @param None
 * @return int 程序返回值
 */
int main(void)
{
    // STM32硬件基础初始化
    SystemInit();  // 初始化系统时钟，配置PLL和时钟树
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  // 配置NVIC中断优先级分组为4，支持4位抢占优先级和0位子优先级

    // 创建事件组用于任务间同步和通信
    xInitEventGroup = xEventGroupCreate();  // 创建初始化同步事件组，用于协调硬件和应用初始化顺序
    xUARTandKey = xEventGroupCreate();      // 创建任务通信事件组，用于串口、按键和定时器事件传递
    xLEDEventGroup = xEventGroupCreate();   // 创建专用LED事件组，用于LED控制任务同步

    // 创建系统初始化任务
    xTaskCreate(vInitHardware,           // 指向任务函数的指针
                "Hardware Init",         // 任务描述名称，用于调试和追踪
                configMINIMAL_STACK_SIZE, // 任务堆栈大小，使用FreeRTOS最小配置堆栈
                NULL,                    // 传递给任务的参数指针，此处为NULL
                6,                       // 任务优先级，数值越大优先级越高，范围0-(configMAX_PRIORITIES-1)
                NULL);                   // 任务句柄指针，用于引用创建的任务，此处不需要

    // 创建应用初始化任务，优先级略低于硬件初始化任务
    xTaskCreate(vInitTask, 
                "Init Task", 
                configMINIMAL_STACK_SIZE, 
                NULL, 
                5,                       // 优先级5，确保在硬件初始化完成后执行
                NULL);

    // 创建周期性软件定时器，用于生成定时事件
    xPeriodicTimer = xTimerCreate(
        "PeriodicTimer",        // 定时器名称字符串，用于调试识别
        pdMS_TO_TICKS(1000),    // 定时周期：1000ms = 1秒，pdMS_TO_TICKS将毫秒转换为系统节拍数
        pdTRUE,                 // 自动重载模式：pdTRUE表示周期定时器，超时后自动重启
        NULL,                   // 定时器ID：用于在回调函数中识别不同定时器，此处为NULL
        vTimerCallback          // 定时器超时回调函数指针
    );
    
    // 启动软件定时器，不等待命令发送完成（xTicksToWait=0）
    xTimerStart(xPeriodicTimer, 0);

    // 启动FreeRTOS调度器，开始多任务执行环境
    // 一旦调用此函数，控制权将交给调度器，不会返回除非调度器启动失败
    vTaskStartScheduler();

    // 调度器启动失败处理代码
    // 正常情况下不会执行到这里，如果执行说明系统启动失败
    while(1) {
        // 系统启动失败处理代码，通常包括错误指示和系统复位
    }
}

/**
 * @brief 硬件初始化任务函数 - 负责所有外设硬件的初始化
 * @param pvParameters 任务参数指针，由xTaskCreate传递，此处未使用
 */
void vInitHardware(void *pvParameters)
{
    // 执行各硬件模块初始化函数
    led_init();        // 初始化LED控制相关GPIO引脚和时钟
    key_init();        // 初始化按键检测GPIO和中断配置
    Serial_Init();     // 初始化串口通信参数：波特率、数据位、停止位等

    // 设置事件标志位，通知等待任务硬件初始化已完成
    // 此操作会唤醒所有等待HARDWARE_INIT_COMPLETE_BIT事件的任务
    xEventGroupSetBits(xInitEventGroup, HARDWARE_INIT_COMPLETE_BIT);

    // 硬件初始化任务完成使命，删除自身释放资源
    // NULL参数表示删除当前执行的任务
    vTaskDelete(NULL);
}

/**
 * @brief 应用任务初始化任务函数 - 等待硬件初始化完成后创建应用任务
 * @param pvParameters 任务参数指针，由xTaskCreate传递，此处未使用
 */
void vInitTask(void *pvParameters)
{
    // 等待硬件初始化完成事件标志
    // 此调用会阻塞当前任务直到指定事件位被设置或超时
    EventBits_t uxBits = xEventGroupWaitBits(
        xInitEventGroup,            // 要等待的事件组句柄
        HARDWARE_INIT_COMPLETE_BIT, // 要等待的特定标志位（位0）
        pdTRUE,                     // 退出前清除等待的标志位：pdTRUE表示清除，pdFALSE保持
        pdTRUE,                     // 等待模式：pdTRUE需要所有指定位都置位，pdFALSE任意位置位即可
        portMAX_DELAY               // 等待时间：portMAX_DELAY表示无限期等待直到事件发生
    );

    // 检查是否成功接收到硬件初始化完成事件
    // uxBits包含事件组当前所有置位的标志位
    if((uxBits & HARDWARE_INIT_COMPLETE_BIT) != 0)
    {
        // 硬件初始化完成，创建所有应用任务
        
        // 创建任务1：LED控制任务1，优先级1（系统最低优先级）
        xTaskCreate(vTask1, 
                    "LED Task", 
                    configMINIMAL_STACK_SIZE, 
                    NULL, 
                    1,                       // 优先级1，确保其他高优先级任务能及时响应
                    &xSuspendableTaskHandle); // 保存任务句柄，便于后续挂起、恢复等操作

        // 创建任务2：LED控制任务2，优先级2
        xTaskCreate(vTask2, 
                    "LED Control", 
                    configMINIMAL_STACK_SIZE, 
                    NULL, 
                    2,                       // 优先级2，高于任务1
                    NULL);                   // 不需要保存任务句柄

        // 创建任务3：按键扫描任务，优先级3
        xTaskCreate(vTask3, 
                    "Key Scan", 
                    configMINIMAL_STACK_SIZE, 
                    NULL, 
                    3,                       // 优先级3，确保按键响应及时性
                    NULL);

        // 创建任务4：串口处理任务，优先级4，分配256字堆栈（大于默认值）
        xTaskCreate(vTaskUART, 
                    "UART Process", 
                    256,                     // 分配256字堆栈，因为串口处理可能需要更多缓冲区
                    NULL, 
                    4,                       // 优先级4，较高的优先级确保串口数据及时处理
                    NULL);
        
        // 系统初始化完成，通过串口发送提示信息
        Serial_SendString("系统初始化完成，应用任务已启动\r\n");
    }
    else
    {
        // 事件等待失败处理，通常是因为超时
        Serial_SendString("错误：硬件初始化超时\r\n");
    }

    // 删除初始化事件组，因为初始化阶段已完成，后续不再需要此事件组
    // 释放事件组占用的系统资源
    vEventGroupDelete(xInitEventGroup);

    // 初始化任务完成所有工作，删除自身释放资源
    vTaskDelete(NULL);
}

/**
 * @brief 任务1：LED控制任务1 - 等待LED事件并控制GPIOE_Pin_6 LED
 * @param pvParameters 任务参数指针，由xTaskCreate传递，此处未使用
 */
void vTask1(void *pvParameters)
{
    // 任务主循环，FreeRTOS任务通常为无限循环
    for(;;)
    { 
        // 等待LED事件组中的LED_BIT标志位
        // 此调用会阻塞任务直到LED_BIT被设置
        EventBits_t uxBits = xEventGroupWaitBits(
            xLEDEventGroup,     // 要等待的事件组句柄
            LED_BIT,            // 要等待的特定标志位（位0）
            pdTRUE,             // 退出前清除等待的标志位
            pdTRUE,             // 需要所有指定标志位都置位
            portMAX_DELAY       // 无限期等待直到事件发生
        );

        // LED事件触发，切换GPIOE_Pin_6引脚状态（LED亮/灭切换）
        GPIO_ToggleBits(GPIOE, GPIO_Pin_6);

        // 任务延时1000ms（1秒）
        // portTICK_RATE_MS是系统节拍周期对应的毫秒数
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

/**
 * @brief 任务3：按键扫描任务 - 检测按键状态变化并触发相应事件
 * @param pvParameters 任务参数指针，由xTaskCreate传递，此处未使用
 */
void vTask3(void *pvParameters)
{
    // 按键状态检测变量
    unsigned char Key_Down = 0;   // 按键按下边缘检测结果，0=无按键，1=按键1，2=按键2
    unsigned char Key_Now = 0;    // 当前按键状态值
    unsigned char Key_Before = 0; // 上一次扫描的按键状态值
    
    // 任务主循环
    for(;;)
    {
        // 按键状态检测逻辑：检测按键按下边缘（从释放到按下的转变）
        Key_Before = Key_Now;                    // 保存上次按键状态用于边缘检测
        Key_Now = key_get();                     // 调用按键驱动获取当前所有按键状态
        Key_Down = Key_Now & (Key_Now ^ Key_Before); // 边缘检测算法：当前状态与上次状态异或后与当前状态与

        // 根据检测到的按键按下事件设置相应事件标志
        if(Key_Down == 1)
        {
            // 按键1按下，在事件组中设置KEY_1_BIT标志位
            // 此操作会唤醒所有等待KEY_1_BIT的任务
            xEventGroupSetBits(xUARTandKey, KEY_1_BIT);
        }
        else if(Key_Down == 2)
        {
            // 按键2按下，在事件组中设置KEY_2_BIT标志位
            xEventGroupSetBits(xUARTandKey, KEY_2_BIT);
        }
        
        // 按键扫描延时10ms，平衡响应速度和CPU占用率
        // 10ms延时提供足够的消抖时间同时保持响应性
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

/**
 * @brief 任务2：LED控制任务2 - 响应多种事件并控制GPIOE_Pin_5 LED
 * @param pvParameters 任务参数指针，由xTaskCreate传递，此处未使用
 */
void vTask2(void *pvParameters)
{
    // 任务主循环
    for(;;)
    {
        // 等待多种事件中的任意一种发生
        // 使用位或操作组合多个事件标志，任务会等待其中任一标志置位
        xEventGroupWaitBits(
            xUARTandKey,                          // 事件组句柄
            UART_BIT | KEY_2_BIT | KEY_1_BIT | Timer_BIT, // 等待多个标志位：串口、按键1、按键2、定时器
            pdTRUE,                               // 退出前清除所有等待的标志位
            pdTRUE,                               // 需要所有指定标志位都置位（实际上这里等待任意一个）
            portMAX_DELAY                         // 无限期等待直到任一指定事件发生
        );
        
        // 事件触发后切换GPIOE_Pin_5引脚LED状态
        GPIO_ToggleBits(GPIOE, GPIO_Pin_5);
        
        // 延时1000ms后设置LED事件，触发任务1的LED控制
        vTaskDelay(1000 / portTICK_RATE_MS);
        
        // 在LED事件组中设置LED_BIT标志，唤醒等待此事件的任务1
        xEventGroupSetBits(xLEDEventGroup, LED_BIT);
    }
}

/**
 * @brief 任务4：串口数据处理任务 - 接收和处理串口数据包
 * @param pvParameters 任务参数指针，由xTaskCreate传递，此处未使用
 */
void vTaskUART(void *pvParameters)
{
    uart_packet_t packet;      // 串口数据包结构体，用于存储接收到的数据
    BaseType_t xResult;        // 函数操作结果状态，用于检查API调用是否成功
    
    // 任务主循环
    for(;;)
    {
        // 等待接收完整的串口数据包
        // Serial_ReceivePacket是自定义的串口数据包接收函数
        xResult = Serial_ReceivePacket(&packet, portMAX_DELAY);
        
        // 检查数据包接收是否成功
        if(xResult == pdTRUE)  // pdTRUE表示成功接收到有效数据包
        {
            // 在事件组中设置UART_BIT标志，通知其他任务有串口数据到达
            xEventGroupSetBits(xUARTandKey, UART_BIT);
            
            // 通过串口打印接收到的数据信息，用于调试和监控
            Serial_Printf("收到串口命令: %s, 数据长度: %d\r\n", packet.data, packet.length);
        }
        else  // 数据包接收失败，可能是格式错误或超时
        {
            // 发送错误信息到串口，提示用户数据接收失败
            Serial_SendString("错误：串口数据包接收失败\r\n");
        }
    }
}

/**
 * @brief 软件定时器回调函数 - 定时器超时时自动调用
 * @param xTimer 触发回调的定时器句柄，用于识别是哪个定时器超时
 */
void vTimerCallback(TimerHandle_t xTimer)
{
    // 定时器超时，在事件组中设置Timer_BIT标志
    // 此操作会唤醒所有等待Timer_BIT事件的任务
    xEventGroupSetBits(xUARTandKey, Timer_BIT);
    
    // 注意：定时器回调函数中不能调用可能导致阻塞的API
    // 此函数在定时器服务任务上下文中执行，应保持简短高效
}
