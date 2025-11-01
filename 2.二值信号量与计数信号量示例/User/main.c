#include "FreeRTOS.h" 
#include "task.h"
#include "stm32f4xx.h"
#include "key.h"
#include "./led/bsp_led.h"   
#include "queue.h"  // 队列的头文件，用于任务间通信

/* 声明任务函数 */
void vTask1(void *pvParameters);  // LED闪烁任务
void vTask2(void *pvParameters);  // 串口打印任务（实际是LED控制任务）
void vTask3(void *pvParameters);  // 按键扫描任务

/* 任务句柄声明 */
TaskHandle_t xSuspendableTaskHandle = NULL;  // 任务1的句柄，用于挂起/恢复操作
TaskHandle_t xControllerTaskHandle = NULL;   // 控制器任务句柄（当前未使用）

QueueHandle_t LED_1_Queue;  // 队列句柄，用于任务间传递按键数据

int main(void)
{
    /* 系统时钟初始化 */
    SystemInit();
    
    /* 设置NVIC优先级分组 - 对FreeRTOS很重要！
       使用优先级分组4，表示4位用于抢占优先级，0位用于子优先级 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    /* 硬件初始化 */
    led_init();   // LED初始化
    key_init();   // 按键初始化
    
    /* 创建任务1 - LED闪烁任务 */
    xTaskCreate(
        vTask1,                    /* 任务函数指针 */
        "LED Task",                /* 任务名称，用于调试标识 */
        configMINIMAL_STACK_SIZE,  /* 堆栈大小，使用FreeRTOS最小配置堆栈 */
        NULL,                      /* 任务参数，这里为NULL */
        1,                         /* 任务优先级，优先级1（较低） */
        &xSuspendableTaskHandle    /* 任务句柄，用于后续挂起/恢复操作 */
    );
    
    /* 创建任务2 - 串口打印任务（实际功能是LED控制） */
    xTaskCreate(
        vTask2,                    /* 任务函数指针 */
        "UART Task",               /* 任务名称 */
        configMINIMAL_STACK_SIZE,  /* 堆栈大小 */
        NULL,                      /* 任务参数 */
        2,                         /* 任务优先级，优先级2（较高） */
        NULL                       /* 任务句柄，这里不需要所以设为NULL */
    );
    
    /* 创建任务3 - 按键扫描任务 */
    xTaskCreate(
        vTask3,                    /* 任务函数指针 */
        "Key Task",                /* 任务名称 */
        configMINIMAL_STACK_SIZE,  /* 堆栈大小 */
        NULL,                      /* 任务参数 */
        2,                         /* 任务优先级，优先级2（与任务2相同） */
        NULL                       /* 任务句柄 */
    );
    
    /* 创建队列：可以存储5个无符号字符(unsigned char) */
    LED_1_Queue = xQueueCreate(5, sizeof(unsigned char));
    
    /* 启动FreeRTOS调度器 - 程序永远不会返回这里 */
    vTaskStartScheduler();
    
    /* 如果调度器启动失败，会执行到这里 */
    while(1)
    {
        /* 启动失败处理 - 通常需要添加错误处理代码 */
    }
}

/**
 * @brief 任务1 - LED闪烁任务
 * @param pvParameters 任务参数（未使用）
 * 
 * 功能：每隔1秒翻转一次LED状态
 * 优先级：1（较低）
 * 特点：可以被任务3挂起和恢复
 */
void vTask1(void *pvParameters)
{
    /* 无限循环，FreeRTOS任务的标准结构 */
    for(;;)
    {
        /* 翻转PE5引脚上的LED状态 */
        GPIO_WriteBit(GPIOE, GPIO_Pin_5, 
                     (BitAction)(1 - (GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5))));
        
        /* 非阻塞延时，延时1000毫秒 */
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

/**
 * @brief 任务3 - 按键扫描任务
 * @param pvParameters 任务参数（未使用）
 * 
 * 功能：
 * 1. 检测按键状态（按下和释放）
 * 2. 将按下的按键值通过队列发送给任务2
 * 3. 当检测到按键2释放时，切换任务1的挂起/恢复状态
 * 优先级：2（较高）
 */
void vTask3(void *pvParameters)
{
    /* 按键状态变量 */
    unsigned char Key_Down = 0;   // 按键按下事件（上升沿）
    unsigned char Key_Up = 0;     // 按键释放事件（下降沿）
    unsigned char Key_Now = 0;    // 当前按键状态
    unsigned char Key_Before = 0; // 上一次按键状态
    eTaskState task1_state;       // 任务1的状态
    
    for(;;)
    {
        /* 更新按键状态 */
        Key_Before = Key_Now;           // 保存上一次按键状态
        Key_Now = key_get();            // 获取当前按键状态
        
        /* 边缘检测：
           Key_Down = 当前为1且上一次为0（上升沿）
           Key_Up = 当前为0且上一次为1（下降沿） */
        Key_Down = Key_Now & (Key_Now ^ Key_Before);
        Key_Up = ~Key_Now & (Key_Now ^ Key_Before);
        
        /* 如果有按键按下事件，发送到队列 */
        if(Key_Down != 0)
        {
            /* 将按键按下事件发送到队列，不等待（0 ticks） */
            xQueueSend(LED_1_Queue, &Key_Down, 0);
        }
        
        /* 如果检测到按键2释放（假设按键2对应值为2） */
        if(Key_Up == 2)
        {
            /* 获取任务1的当前状态 */
            task1_state = eTaskGetState(xSuspendableTaskHandle);
            
            /* 根据任务1状态决定挂起或恢复 */
            if(task1_state == eSuspended)
            {
                /* 如果任务1被挂起，则恢复它 */
                vTaskResume(xSuspendableTaskHandle);
            }
            else
            {
                /* 如果任务1在运行，则挂起它 */
                vTaskSuspend(xSuspendableTaskHandle);
            }
        }
        
        /* 延时10ms，降低CPU占用率并实现按键去抖 */
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

/**
 * @brief 任务2 - LED控制任务（原注释为串口打印任务，实际是LED控制）
 * @param pvParameters 任务参数（未使用）
 * 
 * 功能：
 * 1. 从队列接收按键数据
 * 2. 当接收到数据时，翻转PE6引脚上的LED
 * 优先级：2（较高）
 */
void vTask2(void *pvParameters)
{
    unsigned char LED_State = 0;  // 用于存储接收到的按键状态
    
    for(;;)
    {
        /* 无限等待队列数据 */
        while(1) 
        {
            /* 从队列接收数据，无限等待直到有数据可用 */
            xQueueReceive(LED_1_Queue, &LED_State, portMAX_DELAY);
            
            /* 接收到数据后翻转PE6引脚上的LED */
            GPIO_ToggleBits(GPIOE, GPIO_Pin_6);
        }
    }
}