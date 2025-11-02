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

/* 任务函数声明 */
void vInitHardware(void *pvParameters);    // 硬件初始化任务
void vInitTask(void *pvParameters);        // 应用任务初始化任务
void vTask1(void *pvParameters);           // LED1控制任务
void vTask2(void *pvParameters);           // LED2控制任务
void vTask3(void *pvParameters);           // 按键扫描任务
void vTaskUART(void *pvParameters);        // 串口数据处理任务
void vTimerCallback(TimerHandle_t xTimer); // 定时器回调函数
void vStatsTask(void *pvParameters);       // CPU统计任务

/* 全局任务句柄声明 */
TaskHandle_t xLED1TaskHandle = NULL;       // LED1任务句柄
TaskHandle_t xLED2TaskHandle = NULL;       // LED2任务句柄
TaskHandle_t xKeyTaskHandle = NULL;        // 按键扫描任务句柄
TaskHandle_t xInitTaskHandle = NULL;       // 初始化任务句柄

/* 定时器句柄声明 */
TimerHandle_t xPeriodicTimer = NULL;       // 周期性定时器句柄

/* CPU利用率统计相关变量 */
volatile unsigned long ulHighFrequencyTimerTicks = 0;  // 高精度定时器计数

/**
 * @brief 获取运行时间计数器当前值
 * @return 返回定时器的当前计数值
 * @note 用于FreeRTOS运行时间统计功能
 */
unsigned long getRunTimeCounterValue(void)
{
    return ulHighFrequencyTimerTicks;
}

/**
 * @brief TIM2定时器中断服务函数
 * @note 用于高精度时间统计，每1ms产生一次中断
 *       在中断中递增计数器，为CPU利用率统计提供时间基准
 */
void TIM2_IRQHandler(void)
{
    // 检查是否发生更新中断
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        ulHighFrequencyTimerTicks++;           // 递增计数器
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // 清除中断标志
    }
}

/**
 * @brief 初始化统计定时器
 * @note 配置TIM2定时器为1kHz频率，用于FreeRTOS的运行时间统计
 *       定时器配置：84MHz/84 = 1MHz，1000分频后得到1kHz
 */
void vInitStatsTimer(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 使能TIM2时钟（APB1总线）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 定时器基础配置
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;          // 自动重装载值，1kHz中断频率
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;         // 预分频值，84MHz/84 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;          // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);       // 初始化TIM2
    
    // 使能TIM2更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    // 配置NVIC中断控制器
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;       // TIM2中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;  // 抢占优先级10
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    // 子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       // 使能中断通道
    NVIC_Init(&NVIC_InitStructure);                       // 初始化NVIC
    
    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief 主函数
 * @note 系统启动入口，初始化硬件和创建初始任务
 *       配置中断优先级分组，启动FreeRTOS调度器
 */
int main(void)
{
    /* 硬件基础初始化 */
    SystemInit();                                // 系统时钟初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  // 设置中断优先级分组为4
    
    /* 初始化CPU统计定时器 */
    vInitStatsTimer();                           // 配置TIM2用于运行时间统计
    
    /* 创建初始化任务 */
    xTaskCreate(vInitHardware, "Hardware Init", configMINIMAL_STACK_SIZE, NULL, 6, NULL);
    xTaskCreate(vInitTask, "Init Task", configMINIMAL_STACK_SIZE, NULL, 5, &xInitTaskHandle);

    /* 创建10ms周期性定时器，用于触发按键扫描 */
    xPeriodicTimer = xTimerCreate("PeriodicTimer", pdMS_TO_TICKS(10), pdTRUE, NULL, vTimerCallback);
    if(xPeriodicTimer != NULL) {
        xTimerStart(xPeriodicTimer, 0);          // 启动定时器
    }
      
    /* 启动FreeRTOS调度器，开始多任务调度 */
    vTaskStartScheduler();

    /* 如果调度器启动失败，将进入死循环 */
    while(1) {
        // 系统启动失败处理
    }
}

/**
 * @brief 硬件初始化任务
 * @param pvParameters 任务参数（未使用）
 * @note 初始化所有硬件外设，完成后通知初始化任务继续执行
 */
void vInitHardware(void *pvParameters)
{
    // 初始化各硬件模块
    led_init();           // LED GPIO初始化
    key_init();           // 按键GPIO初始化
    Serial_Init();        // 串口初始化
    
    // 通知初始化任务硬件初始化完成
    xTaskNotifyGive(xInitTaskHandle);
    
    // 删除自身，硬件初始化任务只需执行一次
    vTaskDelete(NULL);
}

/**
 * @brief 应用任务初始化任务
 * @param pvParameters 任务参数（未使用）
 * @note 等待硬件初始化完成，然后创建所有应用任务
 *       使用任务通知进行任务间同步
 */
void vInitTask(void *pvParameters)
{
    // 等待硬件初始化任务完成通知
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    /* 创建所有应用任务 */
    xTaskCreate(vTask1, "LED Task", configMINIMAL_STACK_SIZE, NULL, 1, &xLED1TaskHandle);
    xTaskCreate(vTask2, "LED Control", configMINIMAL_STACK_SIZE, NULL, 2, &xLED2TaskHandle);
    xTaskCreate(vTask3, "Key Scan", configMINIMAL_STACK_SIZE, NULL, 3, &xKeyTaskHandle);
    xTaskCreate(vTaskUART, "UART Process", 256, NULL, 4, NULL);
    
    /* 创建CPU统计任务，优先级1 */
    xTaskCreate(vStatsTask, "CPU Stats", 256, NULL, 1, NULL);
    
    // 发送初始化完成信息
    Serial_SendString("系统初始化完成，应用任务已启动\r\n");
    
    // 添加调试信息，确认统计定时器工作正常
    Serial_Printf("时间统计定时器已启动，当前计数值: %lu\r\n", getRunTimeCounterValue());

    // 初始化任务完成，删除自身
    vTaskDelete(NULL);
}

/**
 * @brief LED1控制任务
 * @param pvParameters 任务参数（未使用）
 * @note 等待任务通知，收到通知后切换LED1状态
 *       使用任务通知实现任务间通信
 */
void vTask1(void *pvParameters)
{
    for(;;)
    { 
        // 等待任务通知，收到通知后切换LED1
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        GPIO_ToggleBits(GPIOE, GPIO_Pin_5);  // 切换PE5引脚（LED1）
    }
}

/**
 * @brief 按键扫描任务
 * @param pvParameters 任务参数（未使用）
 * @note 每10ms执行一次按键扫描，检测按键按下事件
 *       使用状态机方式检测按键的下降沿
 */
void vTask3(void *pvParameters)
{
    unsigned char Key_Down = 0;   // 按键按下标志
    unsigned char Key_Now = 0;    // 当前按键状态
    unsigned char Key_Before = 0; // 上一次按键状态
    
    for(;;)
    {
        // 等待定时器触发（10ms周期）
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        // 按键状态检测状态机
        Key_Before = Key_Now;                     // 保存上次状态
        Key_Now = key_get();                      // 读取当前按键状态
        Key_Down = Key_Now & (Key_Now ^ Key_Before);  // 检测下降沿

        // 按键1按下（Key_Down == 1），通知LED2任务
        if(Key_Down == 1)
        {
            xTaskNotifyGive(xLED2TaskHandle);
        }
        // 按键2按下（Key_Down == 2），通知LED1任务
        if(Key_Down == 2)
        {
            xTaskNotifyGive(xLED1TaskHandle);
        }
    }
}

/**
 * @brief LED2控制任务
 * @param pvParameters 任务参数（未使用）
 * @note 等待任务通知，收到通知后切换LED2状态
 */
void vTask2(void *pvParameters)
{
    for(;;)
    {
        // 等待任务通知，收到通知后切换LED2
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        GPIO_ToggleBits(GPIOE, GPIO_Pin_6);  // 切换PE6引脚（LED2）
    }
}

/**
 * @brief 串口数据处理任务
 * @param pvParameters 任务参数（未使用）
 * @note 接收并处理串口数据包，支持超时等待
 *       使用自定义的串口数据包协议
 */
void vTaskUART(void *pvParameters)
{
    uart_packet_t packet;        // 串口数据包结构体
    BaseType_t xResult;          // 操作结果
    
    for(;;)
    {
        // 等待接收串口数据包（无限期等待）
        xResult = Serial_ReceivePacket(&packet, portMAX_DELAY);
        
        if(xResult == pdTRUE)
        {
            // 成功接收到数据包，打印信息
            Serial_Printf("收到串口命令: %s, 数据长度: %d\r\n", packet.data, packet.length);
        }
        else
        {
            // 接收失败处理
            Serial_SendString("错误：串口数据包接收失败\r\n");
        }
    }
}

/**
 * @brief CPU统计任务
 * @param pvParameters 任务参数（未使用）
 * @note 周期性输出系统任务状态和CPU利用率信息
 *       每1秒输出一次系统运行状态统计
 */
void vStatsTask(void *pvParameters)
{
    uint8_t CPU_RunInfo[512];           // 统计信息缓冲区
    static unsigned long lastCounterValue = 0;  // 上一次定时器计数值
    
    // 等待系统稳定运行2秒后再开始统计
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // 输出实验标题
    Serial_SendString("这是一个STM32-FreeRTOS-CPU利用率统计实验!\r\n");
    
    for(;;)
    {
        unsigned long currentCounter;
        
        // 每1秒执行一次统计
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        // 调试信息：检查定时器是否正常工作
        currentCounter = getRunTimeCounterValue();
        Serial_Printf("定时器计数: %lu, 增量: %lu\r\n", currentCounter, currentCounter - lastCounterValue);
        lastCounterValue = currentCounter;
        
        // 清空缓冲区，准备存储任务列表信息
        memset(CPU_RunInfo, 0, 512);
        
        // 获取任务运行时间信息（任务状态、优先级、栈使用等）
        vTaskList((char *)&CPU_RunInfo);
        
        // 输出任务列表标题和内容
        Serial_SendString("---------------------------------------------\r\n");
        Serial_SendString("任务名      任务状态 优先级 剩余栈 任务序号\r\n");
        Serial_SendString((char *)CPU_RunInfo);
        Serial_SendString("---------------------------------------------\r\n");
        
        // 清空缓冲区，准备存储CPU利用率信息
        memset(CPU_RunInfo, 0, 512);
        
        // 获取CPU利用率统计信息
        vTaskGetRunTimeStats((char *)&CPU_RunInfo);
        
        // 输出CPU利用率标题
        Serial_SendString("任务名      运行计数   使用率\r\n");
        
        // 检查是否有统计数据可用
        if(strlen((char *)CPU_RunInfo) > 0) {
            Serial_SendString((char *)CPU_RunInfo);  // 输出统计信息
        } else {
            Serial_SendString("无统计数据可用\r\n");  // 无数据提示
        }
        
        // 输出统计信息结束分隔线
        Serial_SendString("---------------------------------------------\r\n\r\n");
    }
}

/**
 * @brief 定时器回调函数
 * @param xTimer 定时器句柄
 * @note 每10ms执行一次，用于触发按键扫描任务
 *       通过任务通知机制唤醒按键扫描任务
 */
void vTimerCallback(TimerHandle_t xTimer)
{
    // 发送任务通知给按键扫描任务，触发按键检测
    xTaskNotifyGive(xKeyTaskHandle);
}