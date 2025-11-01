#include "FreeRTOS.h" 
#include "task.h"
#include "stm32f4xx.h"
#include "key.h"
#include "./led/bsp_led.h"   
#include "queue.h"  // ���е�ͷ�ļ������������ͨ��

/* ���������� */
void vTask1(void *pvParameters);  // LED��˸����
void vTask2(void *pvParameters);  // ���ڴ�ӡ����ʵ����LED��������
void vTask3(void *pvParameters);  // ����ɨ������

/* ���������� */
TaskHandle_t xSuspendableTaskHandle = NULL;  // ����1�ľ�������ڹ���/�ָ�����
TaskHandle_t xControllerTaskHandle = NULL;   // ����������������ǰδʹ�ã�

QueueHandle_t LED_1_Queue;  // ���о������������䴫�ݰ�������

int main(void)
{
    /* ϵͳʱ�ӳ�ʼ�� */
    SystemInit();
    
    /* ����NVIC���ȼ����� - ��FreeRTOS����Ҫ��
       ʹ�����ȼ�����4����ʾ4λ������ռ���ȼ���0λ���������ȼ� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    /* Ӳ����ʼ�� */
    led_init();   // LED��ʼ��
    key_init();   // ������ʼ��
    
    /* ��������1 - LED��˸���� */
    xTaskCreate(
        vTask1,                    /* ������ָ�� */
        "LED Task",                /* �������ƣ����ڵ��Ա�ʶ */
        configMINIMAL_STACK_SIZE,  /* ��ջ��С��ʹ��FreeRTOS��С���ö�ջ */
        NULL,                      /* �������������ΪNULL */
        1,                         /* �������ȼ������ȼ�1���ϵͣ� */
        &xSuspendableTaskHandle    /* �����������ں�������/�ָ����� */
    );
    
    /* ��������2 - ���ڴ�ӡ����ʵ�ʹ�����LED���ƣ� */
    xTaskCreate(
        vTask2,                    /* ������ָ�� */
        "UART Task",               /* �������� */
        configMINIMAL_STACK_SIZE,  /* ��ջ��С */
        NULL,                      /* ������� */
        2,                         /* �������ȼ������ȼ�2���ϸߣ� */
        NULL                       /* �����������ﲻ��Ҫ������ΪNULL */
    );
    
    /* ��������3 - ����ɨ������ */
    xTaskCreate(
        vTask3,                    /* ������ָ�� */
        "Key Task",                /* �������� */
        configMINIMAL_STACK_SIZE,  /* ��ջ��С */
        NULL,                      /* ������� */
        2,                         /* �������ȼ������ȼ�2��������2��ͬ�� */
        NULL                       /* ������ */
    );
    
    /* �������У����Դ洢5���޷����ַ�(unsigned char) */
    LED_1_Queue = xQueueCreate(5, sizeof(unsigned char));
    
    /* ����FreeRTOS������ - ������Զ���᷵������ */
    vTaskStartScheduler();
    
    /* �������������ʧ�ܣ���ִ�е����� */
    while(1)
    {
        /* ����ʧ�ܴ��� - ͨ����Ҫ��Ӵ�������� */
    }
}

/**
 * @brief ����1 - LED��˸����
 * @param pvParameters ���������δʹ�ã�
 * 
 * ���ܣ�ÿ��1�뷭תһ��LED״̬
 * ���ȼ���1���ϵͣ�
 * �ص㣺���Ա�����3����ͻָ�
 */
void vTask1(void *pvParameters)
{
    /* ����ѭ����FreeRTOS����ı�׼�ṹ */
    for(;;)
    {
        /* ��תPE5�����ϵ�LED״̬ */
        GPIO_WriteBit(GPIOE, GPIO_Pin_5, 
                     (BitAction)(1 - (GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5))));
        
        /* ��������ʱ����ʱ1000���� */
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

/**
 * @brief ����3 - ����ɨ������
 * @param pvParameters ���������δʹ�ã�
 * 
 * ���ܣ�
 * 1. ��ⰴ��״̬�����º��ͷţ�
 * 2. �����µİ���ֵͨ�����з��͸�����2
 * 3. ����⵽����2�ͷ�ʱ���л�����1�Ĺ���/�ָ�״̬
 * ���ȼ���2���ϸߣ�
 */
void vTask3(void *pvParameters)
{
    /* ����״̬���� */
    unsigned char Key_Down = 0;   // ���������¼��������أ�
    unsigned char Key_Up = 0;     // �����ͷ��¼����½��أ�
    unsigned char Key_Now = 0;    // ��ǰ����״̬
    unsigned char Key_Before = 0; // ��һ�ΰ���״̬
    eTaskState task1_state;       // ����1��״̬
    
    for(;;)
    {
        /* ���°���״̬ */
        Key_Before = Key_Now;           // ������һ�ΰ���״̬
        Key_Now = key_get();            // ��ȡ��ǰ����״̬
        
        /* ��Ե��⣺
           Key_Down = ��ǰΪ1����һ��Ϊ0�������أ�
           Key_Up = ��ǰΪ0����һ��Ϊ1���½��أ� */
        Key_Down = Key_Now & (Key_Now ^ Key_Before);
        Key_Up = ~Key_Now & (Key_Now ^ Key_Before);
        
        /* ����а��������¼������͵����� */
        if(Key_Down != 0)
        {
            /* �����������¼����͵����У����ȴ���0 ticks�� */
            xQueueSend(LED_1_Queue, &Key_Down, 0);
        }
        
        /* �����⵽����2�ͷţ����谴��2��ӦֵΪ2�� */
        if(Key_Up == 2)
        {
            /* ��ȡ����1�ĵ�ǰ״̬ */
            task1_state = eTaskGetState(xSuspendableTaskHandle);
            
            /* ��������1״̬���������ָ� */
            if(task1_state == eSuspended)
            {
                /* �������1��������ָ��� */
                vTaskResume(xSuspendableTaskHandle);
            }
            else
            {
                /* �������1�����У�������� */
                vTaskSuspend(xSuspendableTaskHandle);
            }
        }
        
        /* ��ʱ10ms������CPUռ���ʲ�ʵ�ְ���ȥ�� */
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

/**
 * @brief ����2 - LED��������ԭע��Ϊ���ڴ�ӡ����ʵ����LED���ƣ�
 * @param pvParameters ���������δʹ�ã�
 * 
 * ���ܣ�
 * 1. �Ӷ��н��հ�������
 * 2. �����յ�����ʱ����תPE6�����ϵ�LED
 * ���ȼ���2���ϸߣ�
 */
void vTask2(void *pvParameters)
{
    unsigned char LED_State = 0;  // ���ڴ洢���յ��İ���״̬
    
    for(;;)
    {
        /* ���޵ȴ��������� */
        while(1) 
        {
            /* �Ӷ��н������ݣ����޵ȴ�ֱ�������ݿ��� */
            xQueueReceive(LED_1_Queue, &LED_State, portMAX_DELAY);
            
            /* ���յ����ݺ�תPE6�����ϵ�LED */
            GPIO_ToggleBits(GPIOE, GPIO_Pin_6);
        }
    }
}