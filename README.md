<div align="center">

🎯 **文档持续更新中** | 🚀 **更多内容即将到来**

</div>
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

