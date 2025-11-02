#include "FreeRTOS.h" 
#include "task.h"
#include "stm32f4xx.h"
#include "key.h"
#include "usart1.h"
#include "./led/bsp_led.h"   
#include "queue.h"  
#include "semphr.h"
#include <string.h>

/* 任务函数声明 */
void vTask1(void *pvParameters);  // LED闪烁任务（使用计数信号量）
void vTask2(void *pvParameters);  // LED控制任务（使用二值信号量）
void vTask3(void *pvParameters);  // 按键扫描任务（信号量生产者）
void vTaskUART(void *pvParameters); // 串口数据处理任务

/* 全局变量声明 */
TaskHandle_t xSuspendableTaskHandle = NULL;  // 任务1句柄
TaskHandle_t xControllerTaskHandle = NULL;   // 保留备用

SemaphoreHandle_t LED_S_Handle;  // 二值信号量：控制LED状态切换
SemaphoreHandle_t Key_Num;       // 计数信号量：记录按键事件次数

/**
 * @brief 主函数 - FreeRTOS系统初始化与任务创建
 */
int main(void)
{
    /* 硬件初始化 */
    SystemInit();                           // 系统时钟初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // 设置中断优先级分组（4位抢占优先级）
    led_init();                             // LED GPIO初始化
    key_init();                             // 按键GPIO初始化
    Serial_Init();                          // 串口初始化（必须在任务创建前完成）

    /* 创建信号量 */
    LED_S_Handle = xSemaphoreCreateBinary();    // 创建二值信号量，初始值为0
    Key_Num = xSemaphoreCreateCounting(3, 0);   // 创建计数信号量，最大计数值3，初始值0

    /* 创建任务 */
    // 任务1：LED闪烁任务 - 优先级1（最低）
    xTaskCreate(vTask1, "LED Task", configMINIMAL_STACK_SIZE, NULL, 1, &xSuspendableTaskHandle);
    
    // 任务2：LED控制任务 - 优先级2  
    xTaskCreate(vTask2, "LED Control", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    
    // 任务3：按键扫描任务 - 优先级3
    xTaskCreate(vTask3, "Key Scan", configMINIMAL_STACK_SIZE, NULL, 3, NULL);

    // 任务4：串口数据处理任务 - 优先级4（最高）
    xTaskCreate(vTaskUART, "UART Process", 1024, NULL, 4, NULL);

    /* 启动调度器（程序不会返回） */
    vTaskStartScheduler();

    /* 调度器启动失败处理 */
    while(1) {
        // 系统启动失败，需要处理硬件错误
    }
}

/**
 * @brief 任务1 - LED闪烁任务（信号量消费者）
 * @param pvParameters 任务参数（未使用）
 * 
 * 功能说明：
 * - 周期性闪烁PE5连接的LED
 * - 使用计数信号量Key_Num进行同步
 * - 每次执行前必须成功获取信号量
 * - 信号量操作过程包含完整的状态监控
 */
void vTask1(void *pvParameters)
{
    UBaseType_t key_num_count_before, key_num_count_after, key_num_count_give;
    char msg[100];
    
    Serial_SendString("Task1: LED闪烁任务启动\r\n");
    
    for(;;)
    {
        /* 信号量获取前状态监控 */
        key_num_count_before = uxSemaphoreGetCount(Key_Num);
        sprintf(msg, "[信号量监控] 获取前Key_Num计数: %lu\r\n", key_num_count_before);
        Serial_SendString(msg);
        
        /* 阻塞式获取计数信号量（无限等待） */
        xSemaphoreTake(Key_Num, portMAX_DELAY);
        
        /* 信号量获取后状态监控 */
        key_num_count_after = uxSemaphoreGetCount(Key_Num);
        sprintf(msg, "[信号量监控] 获取后Key_Num计数: %lu\r\n", key_num_count_after);
        Serial_SendString(msg);
        
        /* 执行LED状态翻转（PE5） */
        GPIO_WriteBit(GPIOE, GPIO_Pin_5, 
                     (BitAction)(1 - (GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5))));
        
        /* 释放信号量 */
        xSemaphoreGive(Key_Num);
        
        /* 信号量释放后状态监控 */
        key_num_count_give = uxSemaphoreGetCount(Key_Num);
        sprintf(msg, "[信号量监控] 释放后Key_Num计数: %lu\r\n", key_num_count_give);
        Serial_SendString(msg);
        
        /* 任务延时1000ms */
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

/**
 * @brief 任务3 - 按键扫描任务（信号量生产者）
 * @param pvParameters 任务参数（未使用）
 * 
 * 功能说明：
 * - 扫描按键1和按键2的状态变化
 * - 按键1按下：给出二值信号量LED_S_Handle（触发任务2）
 * - 按键2按下：给出计数信号量Key_Num（触发任务1）
 * - 使用状态机实现按键消抖和边缘检测
 */
void vTask3(void *pvParameters)
{
    unsigned char Key_Down = 0;   // 按键按下事件标志
    unsigned char Key_Now = 0;    // 当前按键状态
    unsigned char Key_Before = 0; // 前次按键状态（用于边缘检测）
    
    UBaseType_t led_s_before, led_s_after, key_num_before, key_num_after;
    char msg[100];
    
    Serial_SendString("Task3: 按键扫描任务启动\r\n");
    
    for(;;)
    {
        /* 按键状态更新与边缘检测 */
        Key_Before = Key_Now;           // 保存历史状态
        Key_Now = key_get();            // 读取当前按键状态
        Key_Down = Key_Now & (Key_Now ^ Key_Before); // 上升沿检测

        /* 按键1处理：触发二值信号量 */
        if(Key_Down == 1)
        {
            /* 二值信号量状态监控 */
            led_s_before = uxSemaphoreGetCount(LED_S_Handle);
            sprintf(msg, "[信号量监控] 给出前LED_S_Handle状态: %lu\r\n", led_s_before);
            Serial_SendString(msg);
            
            xSemaphoreGive(LED_S_Handle);  // 触发LED状态切换
            
            led_s_after = uxSemaphoreGetCount(LED_S_Handle);
            sprintf(msg, "[信号量监控] 给出后LED_S_Handle状态: %lu\r\n", led_s_after);
            Serial_SendString(msg);
            
            /* 尝试获取计数信号量（非阻塞） */
            xSemaphoreTake(Key_Num, 0);
            Serial_SendString("按键1按下 - 触发LED切换\r\n");
        }
        /* 按键2处理：触发计数信号量 */
        else if(Key_Down == 2)
        {
            /* 计数信号量状态监控 */
            key_num_before = uxSemaphoreGetCount(Key_Num);
            sprintf(msg, "[信号量监控] 给出前Key_Num计数: %lu\r\n", key_num_before);
            Serial_SendString(msg);
            
            xSemaphoreGive(Key_Num);  // 允许任务1执行
            
            key_num_after = uxSemaphoreGetCount(Key_Num);
            sprintf(msg, "[信号量监控] 给出后Key_Num计数: %lu\r\n", key_num_after);
            Serial_SendString(msg);
            
            Serial_SendString("按键2按下 - 允许LED闪烁\r\n");
        }
        
        /* 10ms延时用于按键消抖 */
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

/**
 * @brief 任务2 - LED控制任务（二值信号量消费者）
 * @param pvParameters 任务参数（未使用）
 * 
 * 功能说明：
 * - 等待二值信号量LED_S_Handle
 * - 信号量到达后切换PE6连接的LED状态
 * - 使用阻塞方式等待信号量，无超时限制
 */
void vTask2(void *pvParameters)
{
    UBaseType_t led_s_before, led_s_after;
    char msg[100];
    
    Serial_SendString("Task2: LED控制任务启动\r\n");
    
    for(;;)
    {
        /* 信号量等待前状态监控 */
        led_s_before = uxSemaphoreGetCount(LED_S_Handle);
        sprintf(msg, "[信号量监控] 等待前LED_S_Handle状态: %lu\r\n", led_s_before);
        Serial_SendString(msg);
        
        /* 阻塞式等待二值信号量（由按键1触发） */
        xSemaphoreTake(LED_S_Handle, portMAX_DELAY);
        
        /* 信号量获取后状态监控 */
        led_s_after = uxSemaphoreGetCount(LED_S_Handle);
        sprintf(msg, "[信号量监控] 获取后LED_S_Handle状态: %lu\r\n", led_s_after);
        Serial_SendString(msg);
        
        /* 执行LED状态切换（PE6） */
        GPIO_ToggleBits(GPIOE, GPIO_Pin_6);
        Serial_SendString("PE6 LED状态切换完成\r\n");
    }
}

/**
 * @brief 任务4 - 串口数据处理任务
 * @param pvParameters 任务参数（未使用）
 * 
 * 功能说明：
 * - 接收并处理串口数据包
 * - 解析串口命令并执行相应操作
 * - 通过串口回复命令执行结果
 * - 使用阻塞式接收，无限期等待数据
 */
void vTaskUART(void *pvParameters)
{
    uart_packet_t packet;
    BaseType_t xResult;
    
    for(;;)
    {
        /* 阻塞式等待串口数据包 */
        xResult = Serial_ReceivePacket(&packet, portMAX_DELAY);
        
        if(xResult == pdTRUE)
        {
            /* 成功接收数据包，进行命令解析 */
            Serial_Printf("收到串口命令: %s, 数据长度: %d\r\n", packet.data, packet.length);
            // 此处可添加命令解析和执行逻辑
        }
        else
        {
            /* 数据包接收失败处理 */
            Serial_SendString("错误：串口数据包接收失败\r\n");
        }
    }
	}