<div align="center">

🎯 **文档持续更新中** | 🚀 **更多内容即将到来**


# FreeRTOS 任务管理函数源码参考

## 📋 任务创建与删除

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xTaskCreate**<br>`BaseType_t xTaskCreate(`<br>`  TaskFunction_t pvTaskCode,`<br>`  const char * const pcName,`<br>`  configSTACK_DEPTH_TYPE usStackDepth,`<br>`  void *pvParameters,`<br>`  UBaseType_t uxPriority,`<br>`  TaskHandle_t *pvCreatedTask`<br>`);` | - `pvTaskCode`: 任务函数指针<br>- `pcName`: 任务名称字符串<br>- `usStackDepth`: 任务堆栈大小<br>- `pvParameters`: 传递给任务的参数<br>- `uxPriority`: 任务优先级<br>- `pvCreatedTask`: 返回的任务句柄指针 | `pdPASS`: 任务创建成功<br>`errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY`: 内存不足 | 动态创建任务 |
| **xTaskCreateStatic**<br>`TaskHandle_t xTaskCreateStatic(`<br>`  TaskFunction_t pxTaskCode,`<br>`  const char * const pcName,`<br>`  const uint32_t ulStackDepth,`<br>`  void * const pvParameters,`<br>`  UBaseType_t uxPriority,`<br>`  StackType_t * const puxStackBuffer,`<br>`  StaticTask_t * const pxTaskBuffer`<br>`);` | - `pxTaskCode`: 任务函数指针<br>- `pcName`: 任务名称字符串<br>- `ulStackDepth`: 任务堆栈深度<br>- `pvParameters`: 传递给任务的参数<br>- `uxPriority`: 任务优先级<br>- `puxStackBuffer`: 堆栈缓冲区指针<br>- `pxTaskBuffer`: 任务控制块指针 | 任务句柄(成功)<br>`NULL`(失败) | 静态创建任务 |
| **vTaskDelete**<br>`void vTaskDelete(`<br>`  TaskHandle_t xTaskToDelete`<br>`);` | - `xTaskToDelete`: 要删除的任务句柄 | 无 | 删除任务 |

## ⏰ 任务延时控制

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **vTaskDelay**<br>`void vTaskDelay(`<br>`  const TickType_t xTicksToDelay`<br>`);` | - `xTicksToDelay`: 要延时的tick数 | 无 | 相对延时 |
| **vTaskDelayUntil**<br>`void vTaskDelayUntil(`<br>`  TickType_t * const pxPreviousWakeTime,`<br>`  const TickType_t xTimeIncrement`<br>`);` | - `pxPreviousWakeTime`: 上次唤醒时间指针<br>- `xTimeIncrement`: 时间增量(tick数) | 无 | 绝对延时 |

## ⏸️ 任务挂起与恢复

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **vTaskSuspend**<br>`void vTaskSuspend(`<br>`  TaskHandle_t xTaskToSuspend`<br>`);` | - `xTaskToSuspend`: 要挂起的任务句柄 | 无 | 挂起任务 |
| **vTaskResume**<br>`void vTaskResume(`<br>`  TaskHandle_t xTaskToResume`<br>`);` | - `xTaskToResume`: 要恢复的任务句柄 | 无 | 恢复任务 |
| **xTaskResumeFromISR**<br>`BaseType_t xTaskResumeFromISR(`<br>`  TaskHandle_t xTaskToResume`<br>`);` | - `xTaskToResume`: 要恢复的任务句柄 | `pdTRUE`: 需要上下文切换<br>`pdFALSE`: 不需要上下文切换 | 中断中恢复任务 |

## 🎯 优先级管理

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **vTaskPrioritySet**<br>`void vTaskPrioritySet(`<br>`  TaskHandle_t xTask,`<br>`  UBaseType_t uxNewPriority`<br>`);` | - `xTask`: 任务句柄<br>- `uxNewPriority`: 新的优先级值 | 无 | 设置任务优先级 |
| **uxTaskPriorityGet**<br>`UBaseType_t uxTaskPriorityGet(`<br>`  TaskHandle_t xTask`<br>`);` | - `xTask`: 任务句柄 | 任务的当前优先级值 | 获取任务优先级 |

## 🔍 任务状态查询

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **eTaskGetState**<br>`eTaskState eTaskGetState(`<br>`  TaskHandle_t xTask`<br>`);` | - `xTask`: 任务句柄 | 任务状态枚举值 | 获取任务状态 |
| **vTaskGetInfo**<br>`void vTaskGetInfo(`<br>`  TaskHandle_t xTask,`<br>`  TaskStatus_t *pxTaskStatus,`<br>`  BaseType_t xGetFreeStackSpace,`<br>`  eTaskState eState`<br>`);` | - `xTask`: 任务句柄<br>- `pxTaskStatus`: 任务状态结构体指针<br>- `xGetFreeStackSpace`: 是否获取空闲堆栈空间<br>- `eState`: 状态类型 | 无 | 获取任务信息 |

## 🎛️ 调度器控制

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **vTaskStartScheduler**<br>`void vTaskStartScheduler(void);` | 无 | 无 | 启动调度器 |
| **vTaskSuspendAll**<br>`void vTaskSuspendAll(void);` | 无 | 无 | 挂起所有任务 |
| **xTaskResumeAll**<br>`BaseType_t xTaskResumeAll(void);` | 无 | 调度器是否需要上下文切换 | 恢复所有任务 |

## 🛠️ 工具函数

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **uxTaskGetStackHighWaterMark**<br>`UBaseType_t uxTaskGetStackHighWaterMark(`<br>`  TaskHandle_t xTask`<br>`);` | - `xTask`: 任务句柄 | 堆栈高水位线值(剩余最小堆栈空间) | 获取堆栈高水位线 |
| **xTaskGetCurrentTaskHandle**<br>`TaskHandle_t xTaskGetCurrentTaskHandle(void);` | 无 | 当前运行任务的句柄 | 获取当前任务句柄 |
| **pcTaskGetName**<br>`char *pcTaskGetName(`<br>`  TaskHandle_t xTaskToQuery`<br>`);` | - `xTaskToQuery`: 要查询的任务句柄 | 任务名称字符串指针 | 获取任务名称 |
| **xTaskGetTickCount**<br>`TickType_t xTaskGetTickCount(void);` | 无 | 当前的系统tick计数值 | 获取系统tick计数 |
| **xTaskGetTickCountFromISR**<br>`TickType_t xTaskGetTickCountFromISR(void);` | 无 | 当前的系统tick计数值(中断安全版本) | 在ISR中获取tick计数 |

## 🎣 钩子函数

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **vApplicationStackOverflowHook**<br>`void vApplicationStackOverflowHook(`<br>`  TaskHandle_t xTask,`<br>`  char *pcTaskName`<br>`);` | - `xTask`: 发生溢出的任务句柄<br>- `pcTaskName`: 任务名称 | 无 | 堆栈溢出钩子 |
| **vApplicationMallocFailedHook**<br>`void vApplicationMallocFailedHook(void);` | 无 | 无 | 内存分配失败钩子 |
| **vApplicationIdleHook**<br>`void vApplicationIdleHook(void);` | 无 | 无 | 空闲任务钩子 |
| **vApplicationTickHook**<br>`void vApplicationTickHook(void);` | 无 | 无 | 系统tick钩子 |

## 💾 系统任务内存提供

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **vApplicationGetIdleTaskMemory**<br>`void vApplicationGetIdleTaskMemory(`<br>`  StaticTask_t **ppxIdleTaskTCBBuffer,`<br>`  StackType_t **ppxIdleTaskStackBuffer,`<br>`  uint32_t *pulIdleTaskStackSize`<br>`);` | - `ppxIdleTaskTCBBuffer`: 空闲任务TCB缓冲区指针<br>- `ppxIdleTaskStackBuffer`: 空闲任务堆栈缓冲区指针<br>- `pulIdleTaskStackSize`: 空闲任务堆栈大小指针 | 无 | 提供空闲任务内存 |
| **vApplicationGetTimerTaskMemory**<br>`void vApplicationGetTimerTaskMemory(`<br>`  StaticTask_t **ppxTimerTaskTCBBuffer,`<br>`  StackType_t **ppxTimerTaskStackBuffer,`<br>`  uint32_t *pulTimerTaskStackSize`<br>`);` | - `ppxTimerTaskTCBBuffer`: 定时器任务TCB缓冲区指针<br>- `ppxTimerTaskStackBuffer`: 定时器任务堆栈缓冲区指针<br>- `pulTimerTaskStackSize`: 定时器任务堆栈大小指针 | 无 | 提供定时器任务内存 |

# FreeRTOS 消息队列函数参考手册

## 📋 队列创建与删除

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xQueueCreate**<br>`QueueHandle_t xQueueCreate(`<br>`  UBaseType_t uxQueueLength,`<br>`  UBaseType_t uxItemSize`<br>`);` | - `uxQueueLength`: 队列长度<br>- `uxItemSize`: 消息大小(字节) | 队列句柄(成功)<br>`NULL`(失败) | 动态创建队列 |
| **xQueueCreateStatic**<br>`QueueHandle_t xQueueCreateStatic(`<br>`  UBaseType_t uxQueueLength,`<br>`  UBaseType_t uxItemSize,`<br>`  uint8_t *pucQueueStorage,`<br>`  StaticQueue_t *pxQueueBuffer`<br>`);` | - `uxQueueLength`: 队列长度<br>- `uxItemSize`: 消息大小<br>- `pucQueueStorage`: 存储区指针<br>- `pxQueueBuffer`: 队列控制块指针 | 队列句柄(成功)<br>`NULL`(失败) | 静态创建队列 |
| **vQueueDelete**<br>`void vQueueDelete(`<br>`  QueueHandle_t xQueue`<br>`);` | - `xQueue`: 要删除的队列句柄 | 无 | 删除队列 |

## 📤 消息发送

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xQueueSend**<br>`BaseType_t xQueueSend(`<br>`  QueueHandle_t xQueue,`<br>`  const void *pvItemToQueue,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xQueue`: 队列句柄<br>- `pvItemToQueue`: 发送数据指针<br>- `xTicksToWait`: 阻塞时间 | `pdPASS`: 发送成功<br>`errQUEUE_FULL`: 队列满 | 发送消息(队尾) |
| **xQueueSendToFront**<br>`BaseType_t xQueueSendToFront(`<br>`  QueueHandle_t xQueue,`<br>`  const void *pvItemToQueue,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xQueue`: 队列句柄<br>- `pvItemToQueue`: 发送数据指针<br>- `xTicksToWait`: 阻塞时间 | `pdPASS`: 发送成功<br>`errQUEUE_FULL`: 队列满 | 发送消息(队首) |
| **xQueueSendToBack**<br>`BaseType_t xQueueSendToBack(`<br>`  QueueHandle_t xQueue,`<br>`  const void *pvItemToQueue,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xQueue`: 队列句柄<br>- `pvItemToQueue`: 发送数据指针<br>- `xTicksToWait`: 阻塞时间 | `pdPASS`: 发送成功<br>`errQUEUE_FULL`: 队列满 | 发送消息(队尾) |
| **xQueueSendFromISR**<br>`BaseType_t xQueueSendFromISR(`<br>`  QueueHandle_t xQueue,`<br>`  const void *pvItemToQueue,`<br>`  BaseType_t *pxHigherPriorityTaskWoken`<br>`);` | - `xQueue`: 队列句柄<br>- `pvItemToQueue`: 发送数据指针<br>- `pxHigherPriorityTaskWoken`: 任务唤醒标志 | `pdPASS`: 发送成功<br>`errQUEUE_FULL`: 队列满 | 中断中发送消息 |

## 📥 消息接收

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xQueueReceive**<br>`BaseType_t xQueueReceive(`<br>`  QueueHandle_t xQueue,`<br>`  void *pvBuffer,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xQueue`: 队列句柄<br>- `pvBuffer`: 接收缓冲区指针<br>- `xTicksToWait`: 阻塞时间 | `pdPASS`: 接收成功<br>`errQUEUE_EMPTY`: 队列空 | 接收消息 |
| **xQueuePeek**<br>`BaseType_t xQueuePeek(`<br>`  QueueHandle_t xQueue,`<br>`  void *pvBuffer,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xQueue`: 队列句柄<br>- `pvBuffer`: 接收缓冲区指针<br>- `xTicksToWait`: 阻塞时间 | `pdPASS`: 查看成功<br>`errQUEUE_EMPTY`: 队列空 | 查看消息(不删除) |
| **xQueueReceiveFromISR**<br>`BaseType_t xQueueReceiveFromISR(`<br>`  QueueHandle_t xQueue,`<br>`  void *pvBuffer,`<br>`  BaseType_t *pxHigherPriorityTaskWoken`<br>`);` | - `xQueue`: 队列句柄<br>- `pvBuffer`: 接收缓冲区指针<br>- `pxHigherPriorityTaskWoken`: 任务唤醒标志 | `pdPASS`: 接收成功<br>`errQUEUE_EMPTY`: 队列空 | 中断中接收消息 |

## 🔍 队列状态查询

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **uxQueueMessagesWaiting**<br>`UBaseType_t uxQueueMessagesWaiting(`<br>`  QueueHandle_t xQueue`<br>`);` | - `xQueue`: 队列句柄 | 队列中消息数量 | 查询消息数量 |
| **uxQueueSpacesAvailable**<br>`UBaseType_t uxQueueSpacesAvailable(`<br>`  QueueHandle_t xQueue`<br>`);` | - `xQueue`: 队列句柄 | 队列剩余空间数量 | 查询剩余空间 |
| **xQueueIsQueueEmptyFromISR**<br>`BaseType_t xQueueIsQueueEmptyFromISR(`<br>`  QueueHandle_t xQueue`<br>`);` | - `xQueue`: 队列句柄 | `pdTRUE`: 队列空<br>`pdFALSE`: 队列非空 | 中断中查询队列空 |
| **xQueueIsQueueFullFromISR**<br>`BaseType_t xQueueIsQueueFullFromISR(`<br>`  QueueHandle_t xQueue`<br>`);` | - `xQueue`: 队列句柄 | `pdTRUE`: 队列满<br>`pdFALSE`: 队列非满 | 中断中查询队列满 |

## 🛠️ 队列管理

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xQueueReset**<br>`BaseType_t xQueueReset(`<br>`  QueueHandle_t xQueue`<br>`);` | - `xQueue`: 队列句柄 | `pdPASS`: 重置成功<br>`pdFAIL`: 重置失败 | 重置队列 |
| **xQueueOverwrite**<br>`BaseType_t xQueueOverwrite(`<br>`  QueueHandle_t xQueue,`<br>`  const void *pvItemToQueue`<br>`);` | - `xQueue`: 队列句柄<br>- `pvItemToQueue`: 发送数据指针 | 总是返回`pdPASS` | 覆盖发送(用于长度1的队列) |
| **xQueueOverwriteFromISR**<br>`BaseType_t xQueueOverwriteFromISR(`<br>`  QueueHandle_t xQueue,`<br>`  const void *pvItemToQueue,`<br>`  BaseType_t *pxHigherPriorityTaskWoken`<br>`);` | - `xQueue`: 队列句柄<br>- `pvItemToQueue`: 发送数据指针<br>- `pxHigherPriorityTaskWoken`: 任务唤醒标志 | 总是返回`pdPASS` | 中断中覆盖发送 |

# 二值信号量

## 核心概念
**二值信号量 = 事件通知开关 🚦**

- 只有两种状态：0（空）或 1（满）
- 用于任务同步，不传递数据

## 三个关键状态
- **0（空）**：事件未发生，任务获取时会**阻塞**
- **1（满）**：事件已发生，任务可立即获取

## 二值信号量核心函数详细表格

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xSemaphoreCreateBinary**<br>`SemaphoreHandle_t xSemaphoreCreateBinary(void);` | 无参数 | ✅ 成功：有效的二值信号量句柄<br>❌ 失败：NULL（内存不足） | 创建二值信号量<br>• 初始状态为 0（空，不可获取状态）<br>• 使用后必须调用 vSemaphoreDelete() 释放内存<br>• 在堆上动态分配内存<br>• 与互斥量的区别：无优先级继承，无所有权概念<br>• 适合任务同步、事件通知场景 |
| **xSemaphoreTake**<br>`BaseType_t xSemaphoreTake(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xSemaphore`: 二值信号量句柄<br>- `xTicksToWait`: 等待时间配置<br>  - `0`: 不等待，立即返回（轮询模式）<br>  - `portMAX_DELAY`: 无限期等待<br>  - `pdMS_TO_TICKS(ms)`: 等待指定毫秒数<br>  - 具体tick数值: 等待指定系统节拍数 | ✅ `pdPASS`: 成功获取信号量<br>❌ `pdFAIL`: 获取信号量失败或超时 | 获取信号量（P操作）<br>• 信号量=1（满） → 立即成功，状态从1变为0<br>• 信号量=0（空） → 根据等待时间决定是否阻塞<br>• 获取成功后，信号量变为不可用状态<br>• ❌ 不能在中断服务程序中使用<br>• 任何任务都可以获取，无所有权限制 |
| **xSemaphoreGive**<br>`BaseType_t xSemaphoreGive(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 二值信号量句柄 | ✅ `pdPASS`: 成功释放信号量<br>❌ `pdFAIL`: 释放信号量失败 | 释放信号量（V操作）<br>• 信号量=0（空） → 释放成功，状态从0变为1<br>• 信号量=1（满） → 释放失败，状态保持1不变<br>• 如果有任务在等待，会唤醒一个等待任务（FIFO顺序）<br>• ❌ 不能在中断服务程序中使用<br>• 无所有者概念，任何任务都可以释放 |
| **xSemaphoreGiveFromISR**<br>`BaseType_t xSemaphoreGiveFromISR(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  BaseType_t *pxHigherPriorityTaskWoken`<br>`);` | - `xSemaphore`: 二值信号量句柄<br>- `pxHigherPriorityTaskWoken`: 任务切换标志指针<br>  - 必须初始化为 pdFALSE<br>  - 用于指示是否有更高优先级任务被唤醒 | ✅ `pdPASS`: 成功释放信号量<br>❌ `pdFAIL`: 释放信号量失败 | 中断服务程序中释放信号量<br>• 必须使用此版本，不能在中断中使用 xSemaphoreGive<br>• 信号量=0（空） → 释放成功，状态从0变为1<br>• 释放后会检查是否有更高优先级任务被唤醒<br>• 如果 pxHigherPriorityTaskWoken == pdTRUE，必须调用 portYIELD_FROM_ISR()<br>• ❌ 不能在中断中获取信号量 |
| **uxSemaphoreGetCount**<br>`UBaseType_t uxSemaphoreGetCount(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 二值信号量句柄 | `0`: 信号量为空（不可获取）<br>`1`: 信号量为满（可获取） | 获取信号量当前可用状态<br>• 用于调试和状态检查<br>• 不影响信号量的实际状态<br>• 线程安全和中断安全<br>• 对于二值信号量，返回值只能是 0 或 1 |
| **vSemaphoreDelete**<br>`void vSemaphoreDelete(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 要删除的二值信号量句柄 | 无返回值 | 删除二值信号量并释放内存<br>• 删除后信号量句柄失效，不能再使用<br>• 必须确保没有任务正在等待该信号量<br>• 如果还有任务在等待，会导致未定义行为<br>• 删除操作不可逆 |

## 📝 使用示例

```c
// 创建二值信号量
SemaphoreHandle_t xBinarySem = xSemaphoreCreateBinary();
if(xBinarySem == NULL) { 
    // 内存不足处理
}

// 注意：创建后需要先调用 xSemaphoreGive() 才能被获取
xSemaphoreGive(xBinarySem);

// 获取信号量
if(xSemaphoreTake(xBinarySem, pdMS_TO_TICKS(100)) == pdPASS) {
    // 事件已发生，执行相应操作
    // ...
    
    // 处理完成后可以再次释放信号量
    // xSemaphoreGive(xBinarySem);
}

// 中断中释放信号量
void ISR_Handler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xSemaphoreGiveFromISR(xBinarySem, &xHigherPriorityTaskWoken) == pdPASS) {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// 查询信号量状态
UBaseType_t status = uxSemaphoreGetCount(xBinarySem);
if(status == 1) {
    printf("信号量可用，可以立即获取\n");
} else {
    printf("信号量已被占用，需要等待\n");
}

// 安全删除
vSemaphoreDelete(xBinarySem);
xBinarySem = NULL;
```

## ⚠️ 重要说明
**阻塞**：任务等待信号量从0变1时的状态，任务会被挂起直到信号量可用或超时


# 计数信号量 

## 核心概念
**计数信号量 = 资源计数器 🔢**

- 计数值范围：0 到 N（N > 1）
- 用于资源管理和事件计数
- 计数值 = 当前可用资源数量

## 三个关键状态
- **0**：资源耗尽，任务获取时会**阻塞**
- **1 到 N-1**：有部分资源可用
- **N**：资源全满，释放操作会失败

## 计数信号量核心函数详细表格

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xSemaphoreCreateCounting**<br>`SemaphoreHandle_t xSemaphoreCreateCounting(`<br>`  UBaseType_t uxMaxCount,`<br>`  UBaseType_t uxInitialCount`<br>`);` | - `uxMaxCount`: 最大计数值<br>  - 范围：1 到 65535<br>  - 推荐值：根据实际资源数量设置<br>  - 限制：不能为0<br>- `uxInitialCount`: 初始计数值<br>  - 范围：0 到 uxMaxCount<br>  - 常用设置：<br>    - `0`：初始无可用资源<br>    - `uxMaxCount`：初始所有资源可用<br>    - 中间值：初始部分资源可用 | ✅ 成功：有效的计数信号量句柄<br>❌ 失败：NULL（内存不足或参数无效） | 创建计数信号量<br>• 初始状态：uxInitialCount 个可用资源<br>• 最大容量：uxMaxCount 个资源<br>• 用于管理有限数量的资源（如缓冲区槽位、连接数等）<br>• 在堆上动态分配内存<br>• 使用后必须调用 vSemaphoreDelete() 释放内存 |
| **xSemaphoreTake**<br>`BaseType_t xSemaphoreTake(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xSemaphore`: 计数信号量句柄<br>- `xTicksToWait`: 等待时间配置<br>  - `0`: 不等待，立即返回<br>  - `portMAX_DELAY`: 无限期等待<br>  - `pdMS_TO_TICKS(ms)`: 等待指定毫秒数<br>  - 具体tick数值: 等待指定系统节拍数 | ✅ `pdPASS`: 成功获取资源<br>❌ `pdFAIL`: 获取资源失败或超时 | 获取一个资源（消费操作）<br>• 计数值 > 0 → 立即成功，计数值减1<br>• 计数值 = 0 → 根据等待时间决定是否阻塞<br>• 多个任务可以同时获取，直到资源耗尽<br>• ❌ 不能在中断服务程序中使用 |
| **xSemaphoreGive**<br>`BaseType_t xSemaphoreGive(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 计数信号量句柄 | ✅ `pdPASS`: 成功释放资源<br>❌ `pdFAIL`: 释放失败（已达最大容量） | 释放一个资源（生产操作）<br>• 计数值 < uxMaxCount → 释放成功，计数值加1<br>• 计数值 = uxMaxCount → 释放失败，计数值不变<br>• 如果有任务在等待资源，会唤醒一个等待任务<br>• ❌ 不能在中断服务程序中使用 |
| **xSemaphoreGiveFromISR**<br>`BaseType_t xSemaphoreGiveFromISR(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  BaseType_t *pxHigherPriorityTaskWoken`<br>`);` | - `xSemaphore`: 计数信号量句柄<br>- `pxHigherPriorityTaskWoken`: 任务切换标志指针<br>  - 必须初始化为 pdFALSE<br>  - 用于指示是否需要任务切换 | ✅ `pdPASS`: 成功释放资源<br>❌ `pdFAIL`: 释放失败（已达最大容量） | 中断服务程序中释放资源<br>• 必须使用此版本，不能在中断中使用 xSemaphoreGive<br>• 计数值 < uxMaxCount → 释放成功，计数值加1<br>• 释放后会检查是否有更高优先级任务被唤醒<br>• 如果 pxHigherPriorityTaskWoken == pdTRUE，必须调用 portYIELD_FROM_ISR()<br>• ❌ 不能在中断中获取信号量 |
| **uxSemaphoreGetCount**<br>`UBaseType_t uxSemaphoreGetCount(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 计数信号量句柄 | `0` 到 `uxMaxCount`：当前可用资源数量<br>• `0`：无可用资源<br>• `1` 到 `uxMaxCount-1`：部分资源可用<br>• `uxMaxCount`：所有资源可用 | 获取信号量当前计数值<br>• 用于调试和状态检查<br>• 不影响信号量的实际状态<br>• 可以在任务和中断中使用<br>• 反映当前可立即获取的资源数量 |
| **vSemaphoreDelete**<br>`void vSemaphoreDelete(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 要删除的计数信号量句柄 | 无返回值 | 删除计数信号量并释放内存<br>• 删除后信号量句柄失效，不能再使用<br>• 必须确保没有任务正在等待或使用该信号量<br>• 如果还有任务在等待，会导致未定义行为<br>• 删除前应确保所有资源已正确释放 |

## 📝 使用示例

```c
// 创建计数信号量
SemaphoreHandle_t xCountingSem = xSemaphoreCreateCounting(10, 5);
if(xCountingSem == NULL) { 
    // 创建失败处理
}

// 获取资源
if(xSemaphoreTake(xCountingSem, pdMS_TO_TICKS(100)) == pdPASS) {
    // 使用资源
    // ...
    
    // 释放资源
    xSemaphoreGive(xCountingSem);
}

// 中断中释放资源
void ISR_Handler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xCountingSem, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// 查询可用资源
UBaseType_t available = uxSemaphoreGetCount(xCountingSem);
printf("当前可用资源：%d/10\n", available);

// 安全删除
vSemaphoreDelete(xCountingSem);
xCountingSem = NULL;
```
## ⚠️ 重要说明
**阻塞**：当计数值为0时，任务获取信号量会被挂起，直到有资源释放或超时

**资源管理**：计数值表示当前可用资源数量，获取时-1，释放时+1

**事件计数**：也可用于统计事件发生次数，每次事件发生释放信号量


🎯 **适用于：缓冲区管理、线程池、资源池** 

# 互斥信号量 

## 核心概念
**互斥信号量 = 资源锁 🔒**

- 只有两种状态：0（被锁定）或 1（可用）
- 具有优先级继承机制，防止优先级反转
- 用于保护临界资源，确保独占访问

## 三个关键状态
- **0（被锁定）**：资源被占用，任务获取时会**阻塞**
- **1（可用）**：资源空闲，任务可立即获取

## 互斥信号量核心函数详细表格

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xSemaphoreCreateMutex**<br>`SemaphoreHandle_t xSemaphoreCreateMutex(void);` | 无参数 | ✅ 成功：有效的互斥信号量句柄<br>❌ 失败：NULL（内存不足） | 创建互斥信号量<br>• 初始状态为 1（可用状态）<br>• 具有优先级继承特性（自动防止优先级反转）<br>• 在堆上动态分配内存<br>• 使用后必须调用 vSemaphoreDelete() 释放内存<br>• 与二值信号量的区别：具有所有权概念和优先级继承 |
| **xSemaphoreTake**<br>`BaseType_t xSemaphoreTake(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xSemaphore`: 互斥信号量句柄<br>- `xTicksToWait`: 等待时间配置<br>  - `0`: 不等待，立即返回<br>  - `portMAX_DELAY`: 无限期等待<br>  - `pdMS_TO_TICKS(ms)`: 等待指定毫秒数<br>  - 具体tick数值: 等待指定系统节拍数 | ✅ `pdPASS`: 成功获取互斥锁<br>❌ `pdFAIL`: 获取互斥锁失败或超时 | 获取互斥锁<br>• 互斥量=1（可用） → 立即成功，状态从1变为0，任务成为持有者<br>• 互斥量=0（被占用） → 根据等待时间决定是否阻塞<br>• 具有优先级继承：低优先级任务持有锁时，会临时提升优先级<br>• ❌ 不能在中断服务程序中使用<br>• 同一任务不能重复获取（会死锁） |
| **xSemaphoreGive**<br>`BaseType_t xSemaphoreGive(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 互斥信号量句柄<br>- 必须由持有该互斥量的任务调用 | ✅ `pdPASS`: 成功释放互斥锁<br>❌ `pdFAIL`: 释放失败（非持有者调用或未获取） | 释放互斥锁<br>• 必须由持有互斥量的任务释放（所有权概念）<br>• 互斥量=0（被占用） → 释放成功，状态从0变为1<br>• 互斥量=1（可用） → 释放失败，状态保持1不变<br>• 如果有任务在等待，会唤醒优先级最高的等待任务<br>• ❌ 不能在中断服务程序中使用<br>• 恢复优先级继承：释放后恢复任务原始优先级 |
| **uxSemaphoreGetCount**<br>`UBaseType_t uxSemaphoreGetCount(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 互斥信号量句柄 | `0`: 互斥量被占用（不可获取）<br>`1`: 互斥量可用（可获取） | 获取互斥量当前可用状态<br>• 用于调试和状态检查<br>• 不影响互斥量的实际状态<br>• 不反映持有者信息，只反映是否可用<br>• 可以在任务和中断中使用 |
| **vSemaphoreDelete**<br>`void vSemaphoreDelete(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 要删除的互斥信号量句柄 | 无返回值 | 删除互斥信号量并释放内存<br>• 删除后互斥量句柄失效，不能再使用<br>• 必须确保没有任务正在等待或使用该互斥量<br>• 如果还有任务在持有或等待互斥量，会导致未定义行为<br>• 删除前应确保所有任务都已正确释放互斥锁 |
## 📝 使用示例

```c
// 创建互斥信号量
SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();
if(xMutex == NULL) { 
    // 内存不足处理
}

// 获取互斥锁
if(xSemaphoreTake(xMutex, pdMS_TO_TICKS(100)) == pdPASS) {
    // 临界区代码
    // ...
    
    // 释放互斥锁
    xSemaphoreGive(xMutex);
}

// 状态检查
if(uxSemaphoreGetCount(xMutex) == 1) { 
    // 互斥量可用
} else { 
    // 互斥量被占用
}

// 安全删除
vSemaphoreDelete(xMutex);
xMutex = NULL;
```

## ⚠️ 重要说明
**阻塞**：当互斥量被其他任务持有时，任务获取互斥量会被挂起，直到互斥量释放或超时

**优先级继承**：当高优先级任务等待互斥量时，系统会临时提升持有者任务的优先级，防止优先级反转

**所有权**：只有获取互斥量的任务才能释放它，其他任务释放会导致未定义行为

**中断限制**：互斥信号量不能在中断中使用（包括获取和释放）

<div align="center">

🎯 **适用于：临界区保护、共享资源保护** | 🚀 **防止优先级反转**

# 递归互斥量 

## 核心概念
**递归互斥量 = 可重入锁 🔑**

- 允许同一任务多次获取同一个互斥量
- 必须释放相同次数才能真正释放
- 具有优先级继承机制
- 解决"自己锁死自己"的问题

## 三个关键状态
- **0（被锁定）**：互斥量被占用，其他任务获取会**阻塞**
- **1（可用）**：互斥量空闲，任务可立即获取
- **内部计数**：记录同一任务的获取次数

## 递归互斥量核心函数详细表格

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xSemaphoreCreateRecursiveMutex**<br>`SemaphoreHandle_t xSemaphoreCreateRecursiveMutex(void);` | 无参数 | ✅ 成功：有效的递归互斥量句柄<br>❌ 失败：NULL（内存不足） | 创建递归互斥量<br>• 初始状态为 1（可用状态）<br>• 内部递归计数初始为 0<br>• 具有优先级继承特性（防止优先级反转）<br>• 在堆上动态分配内存<br>• 使用后必须调用 vSemaphoreDelete() 释放内存 |
| **xSemaphoreTakeRecursive**<br>`BaseType_t xSemaphoreTakeRecursive(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xSemaphore`: 递归互斥量句柄<br>- `xTicksToWait`: 等待时间配置<br>  - `0`: 不等待，立即返回<br>  - `portMAX_DELAY`: 无限期等待<br>  - `pdMS_TO_TICKS(ms)`: 等待指定毫秒数<br>  - 具体tick数值: 等待指定系统节拍数 | ✅ `pdPASS`: 成功获取递归互斥量<br>❌ `pdFAIL`: 获取递归互斥量失败或超时 | 递归获取互斥锁<br>• 首次获取：状态从1变为0，内部递归计数=1<br>• 同一任务重复获取：内部递归计数+1，不会阻塞<br>• 其他任务获取：会阻塞等待，直到互斥量完全释放<br>• ❌ 不能在中断服务程序中使用 |
| **xSemaphoreGiveRecursive**<br>`BaseType_t xSemaphoreGiveRecursive(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 递归互斥量句柄<br>- 必须由持有该互斥量的任务调用 | ✅ `pdPASS`: 成功释放递归互斥量<br>❌ `pdFAIL`: 释放失败（非持有者调用或未获取） | 递归释放互斥锁<br>• 必须由持有互斥量的任务释放<br>• 部分释放：内部递归计数-1，状态保持0<br>• 完全释放：内部递归计数减到0，状态从0变为1<br>• 释放次数必须与获取次数完全匹配<br>• ❌ 不能在中断服务程序中使用 |
| **uxSemaphoreGetCount**<br>`UBaseType_t uxSemaphoreGetCount(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 递归互斥量句柄 | `0`: 互斥量被占用（不可获取）<br>`1`: 互斥量可用（可获取） | 获取递归互斥量当前可用状态<br>• 不反映内部递归计数，只反映是否可用<br>• 用于调试和状态检查<br>• 不影响互斥量的实际状态<br>• 可以在任务和中断中使用 |
| **vSemaphoreDelete**<br>`void vSemaphoreDelete(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 要删除的递归互斥量句柄 | 无返回值 | 删除递归互斥量并释放内存<br>• 删除后互斥量句柄失效，不能再使用<br>• 必须确保没有任务正在等待或使用该互斥量<br>• 如果还有任务在持有或等待互斥量，会导致未定义行为 |

## 📝 使用示例

```c
// 创建递归互斥量
SemaphoreHandle_t xMutex = xSemaphoreCreateRecursiveMutex();
if(xMutex == NULL) { 
    // 内存不足处理
}

// 递归获取示例
xSemaphoreTakeRecursive(xMutex, portMAX_DELAY);
xSemaphoreTakeRecursive(xMutex, portMAX_DELAY); // 嵌套获取

// 递归释放示例
xSemaphoreGiveRecursive(xMutex); // 释放一次
xSemaphoreGiveRecursive(xMutex); // 完全释放

// 状态检查
if(uxSemaphoreGetCount(xMutex) == 1) { 
    // 互斥量可用
} else { 
    // 互斥量被占用
}

// 安全删除
vSemaphoreDelete(xMutex);
xMutex = NULL;
```
## ⚠️ 重要说明
**阻塞**：当递归互斥量被其他任务持有时，任务获取互斥量会被挂起，直到互斥量释放或超时

**内部计数**：记录同一任务的获取次数，获取时+1，释放时-1，计数=0时真正释放

**匹配释放**：必须调用相同次数的GiveRecursive和TakeRecursive，否则会导致未定义行为

**优先级继承**：当高优先级任务等待递归互斥量时，系统会临时提升持有者任务的优先级

**中断限制**：递归互斥量不能在中断中使用（包括获取和释放）

<div align="center">

🎯 **适用于：递归函数、复杂调用链、面向对象设计** | 🚀 **防止自死锁**
# FreeRTOS 事件组简易笔记

## 🎯 基本概念
**事件组**：32位变量，每个位代表一个独立事件

**事件标志**：事件组中的特定位，表示特定条件

**广播特性**：一个事件可唤醒多个等待任务

## 📋 核心函数速查

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| **xEventGroupCreate**<br>`EventGroupHandle_t xEventGroupCreate(void);` | 无参数 | ✅ 成功：有效的事件组句柄<br>❌ 失败：NULL（内存不足） | 创建新的事件组<br>• 所有事件位初始化为 0（未设置）<br>• 在堆上动态分配内存<br>• 使用后必须调用 vEventGroupDelete() 释放内存 |
| **xEventGroupSetBits**<br>`EventBits_t xEventGroupSetBits(`<br>`  EventGroupHandle_t xEventGroup,`<br>`  const EventBits_t uxBitsToSet`<br>`);` | - `xEventGroup`: 事件组句柄<br>- `uxBitsToSet`: 要设置的事件位掩码 | `EventBits_t`: 设置后的事件组值 | 在任务中设置事件位<br>• 原子操作，不会被其他任务或中断打断<br>• 设置后会自动检查等待这些事件位的任务<br>• ❌ 不能在中断服务程序中使用 |
| **xEventGroupSetBitsFromISR**<br>`BaseType_t xEventGroupSetBitsFromISR(`<br>`  EventGroupHandle_t xEventGroup,`<br>`  const EventBits_t uxBitsToSet,`<br>`  BaseType_t *pxHigherPriorityTaskWoken`<br>`);` | - `xEventGroup`: 事件组句柄<br>- `uxBitsToSet`: 要设置的事件位掩码<br>- `pxHigherPriorityTaskWoken`: 任务切换标志指针 | ✅ `pdPASS`: 命令成功发送到守护任务<br>❌ `pdFAIL`: 命令发送失败（队列满） | 中断服务程序中设置事件位<br>• 必须使用此版本<br>• 通过守护任务队列在任务上下文中执行实际设置<br>• 如果 pxHigherPriorityTaskWoken == pdTRUE，必须调用 portYIELD_FROM_ISR() |
| **xEventGroupWaitBits**<br>`EventBits_t xEventGroupWaitBits(`<br>`  EventGroupHandle_t xEventGroup,`<br>`  const EventBits_t uxBitsToWaitFor,`<br>`  const BaseType_t xClearOnExit,`<br>`  const BaseType_t xWaitForAllBits,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xEventGroup`: 事件组句柄<br>- `uxBitsToWaitFor`: 等待的事件位掩码<br>- `xClearOnExit`: 退出时清除选项<br>- `xWaitForAllBits`: 等待条件<br>- `xTicksToWait`: 等待时间 | `EventBits_t`: 等待结束时的事件组值 | 等待指定事件位被设置（核心函数）<br>• 任务会阻塞直到条件满足或超时<br>• ❌ 不能在中断服务程序中使用 |
| **xEventGroupSync**<br>`EventBits_t xEventGroupSync(`<br>`  EventGroupHandle_t xEventGroup,`<br>`  const EventBits_t uxBitsToSet,`<br>`  const EventBits_t uxBitsToWaitFor,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xEventGroup`: 事件组句柄<br>- `uxBitsToSet`: 本任务要设置的事件位<br>- `uxBitsToWaitFor`: 要等待的事件位<br>- `xTicksToWait`: 等待时间 | `EventBits_t`: 同步完成时的事件组值 | 多任务同步专用函数<br>• 自动执行：设置自己的位 + 等待所有指定位<br>• 专为任务同步点设计，比手动组合更简洁<br>• ❌ 不能在中断服务程序中使用 |
| **xEventGroupClearBits**<br>`EventBits_t xEventGroupClearBits(`<br>`  EventGroupHandle_t xEventGroup,`<br>`  const EventBits_t uxBitsToClear`<br>`);` | - `xEventGroup`: 事件组句柄<br>- `uxBitsToClear`: 要清除的事件位掩码 | `EventBits_t`: 清除前的事件组值 | 在任务中清除事件位<br>• 手动重置事件状态<br>• 不影响等待这些事件位的任务<br>• ❌ 不能在中断服务程序中使用 |
| **xEventGroupClearBitsFromISR**<br>`BaseType_t xEventGroupClearBitsFromISR(`<br>`  EventGroupHandle_t xEventGroup,`<br>`  const EventBits_t uxBitsToClear`<br>`);` | - `xEventGroup`: 事件组句柄<br>- `uxBitsToClear`: 要清除的事件位掩码 | ✅ `pdPASS`: 命令成功发送到守护任务<br>❌ `pdFAIL`: 命令发送失败 | 中断服务程序中清除事件位<br>• 必须使用此版本<br>• 通过守护任务队列在任务上下文中执行实际清除<br>• 不需要任务切换标志（清除操作不会唤醒任务） |
| **xEventGroupGetBitsFromISR**<br>`EventBits_t xEventGroupGetBitsFromISR(`<br>`  EventGroupHandle_t xEventGroup`<br>`);` | - `xEventGroup`: 事件组句柄 | `EventBits_t`: 当前事件组值 | 安全获取事件组当前值<br>• 线程安全和中断安全<br>• 立即返回，不会阻塞<br>• 不影响事件组的状态<br>• 可在任务和中断中使用 |
| **vEventGroupDelete**<br>`void vEventGroupDelete(`<br>`  EventGroupHandle_t xEventGroup`<br>`);` | - `xEventGroup`: 要删除的事件组句柄 | 无返回值 | 删除事件组并释放内存<br>• 删除后事件组句柄失效，不能再使用<br>• 必须确保没有任务正在等待该事件组<br>• 如果还有任务在等待，会导致未定义行为 |

## 📝 使用示例

```c
// 创建事件组
EventGroupHandle_t xEventGroup = xEventGroupCreate();

// 设置事件位
xEventGroupSetBits(xEventGroup, BIT_0 | BIT_1);

// 中断中设置事件位
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
xEventGroupSetBitsFromISR(xEventGroup, BIT_2, &xHigherPriorityTaskWoken);
portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

// 等待事件位
EventBits_t uxBits = xEventGroupWaitBits(
    xEventGroup,           // 事件组句柄
    BIT_0 | BIT_1,         // 等待的位
    pdTRUE,                // 成功后清除这些位
    pdFALSE,               // 任意一个位设置即可
    portMAX_DELAY          // 无限等待
);

// 任务同步
xEventGroupSync(
    xEventGroup,           // 事件组句柄
    BIT_0,                 // 本任务设置的位
    BIT_0 | BIT_1 | BIT_2, // 等待所有任务就绪
    1000                   // 等待1秒
);
```
