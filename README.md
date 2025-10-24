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
| **xSemaphoreCreateBinary**<br>`SemaphoreHandle_t xSemaphoreCreateBinary(void);` | 无 | ✅ 成功：有效句柄<br>❌ 失败：NULL | 创建二值信号量<br>初始状态为 0（空）<br>需要配合 vSemaphoreDelete() 释放内存 |
| **xSemaphoreTake**<br>`BaseType_t xSemaphoreTake(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xSemaphore`: 信号量句柄<br>- `xTicksToWait`: 等待时间<br>  - `0`: 不等待<br>  - `portMAX_DELAY`: 无限等待<br>  - 具体数值: 等待指定tick数 | ✅ `pdTRUE`: 获取成功<br>❌ `pdFALSE`: 获取失败 | 获取信号量<br>信号量=1 → 立即成功，状态1→0<br>信号量=0 → 根据等待时间决定是否**阻塞**<br>不能在中断中使用 |
| **xSemaphoreGive**<br>`BaseType_t xSemaphoreGive(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 信号量句柄 | ✅ `pdTRUE`: 释放成功<br>❌ `pdFALSE`: 释放失败 | 释放信号量<br>信号量=0 → 释放成功，状态0→1<br>信号量=1 → 释放失败，状态不变<br>唤醒一个等待的任务 |
| **xSemaphoreGiveFromISR**<br>`BaseType_t xSemaphoreGiveFromISR(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  BaseType_t *pxHigherPriorityTaskWoken`<br>`);` | - `xSemaphore`: 信号量句柄<br>- `pxHigherPriorityTaskWoken`: 指向任务切换标志的指针 | ✅ `pdTRUE`: 释放成功<br>❌ `pdFALSE`: 释放失败 | 中断专用释放函数<br>需要检查 pxHigherPriorityTaskWoken<br>如果为 pdTRUE，需调用 portYIELD_FROM_ISR(pdTRUE)<br>不能在中断中获取信号量 |
| **uxSemaphoreGetCount**<br>`UBaseType_t uxSemaphoreGetCount(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 信号量句柄 | `0`: 信号量为空<br>`1`: 信号量为满 | 获取信号量当前状态<br>用于调试和状态检查<br>不影响信号量状态 |
| **vSemaphoreDelete**<br>`void vSemaphoreDelete(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 要删除的信号量句柄 | 无 | 删除信号量，释放内存<br>删除后句柄失效，不能再使用<br>确保没有任务在使用该信号量 |

## ⚠️ 重要说明
**阻塞**：任务等待信号量从0变1时的状态，任务会被挂起直到信号量可用或超时

<div align="center">

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
| **xSemaphoreCreateCounting**<br>`SemaphoreHandle_t xSemaphoreCreateCounting(`<br>`  UBaseType_t uxMaxCount,`<br>`  UBaseType_t uxInitialCount`<br>`);` | - `uxMaxCount`: 最大计数值<br>- `uxInitialCount`: 初始计数值 | ✅ 成功：有效句柄<br>❌ 失败：NULL | 创建计数信号量<br>初始状态为 uxInitialCount<br>最大计数值为 uxMaxCount |
| **xSemaphoreTake**<br>`BaseType_t xSemaphoreTake(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xSemaphore`: 信号量句柄<br>- `xTicksToWait`: 等待时间<br>  - `0`: 不等待<br>  - `portMAX_DELAY`: 无限等待<br>  - 具体数值: 等待指定tick数 | ✅ `pdTRUE`: 获取成功<br>❌ `pdFALSE`: 获取失败 | 获取一个资源<br>计数值>0 → 立即成功，计数值-1<br>计数值=0 → 根据等待时间决定是否**阻塞** |
| **xSemaphoreGive**<br>`BaseType_t xSemaphoreGive(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 信号量句柄 | ✅ `pdTRUE`: 释放成功<br>❌ `pdFALSE`: 释放失败 | 释放一个资源<br>计数值<最大值 → 释放成功，计数值+1<br>计数值=最大值 → 释放失败，计数值不变 |
| **xSemaphoreGiveFromISR**<br>`BaseType_t xSemaphoreGiveFromISR(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  BaseType_t *pxHigherPriorityTaskWoken`<br>`);` | - `xSemaphore`: 信号量句柄<br>- `pxHigherPriorityTaskWoken`: 指向任务切换标志的指针 | ✅ `pdTRUE`: 释放成功<br>❌ `pdFALSE`: 释放失败 | 中断专用释放函数<br>计数值<最大值 → 释放成功，计数值+1<br>需要检查任务切换标志 |
| **uxSemaphoreGetCount**<br>`UBaseType_t uxSemaphoreGetCount(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 信号量句柄 | 0 到 N：当前可用资源数量 | 获取信号量当前计数值<br>用于调试和状态检查<br>不影响信号量状态 |
| **vSemaphoreDelete**<br>`void vSemaphoreDelete(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 要删除的信号量句柄 | 无 | 删除信号量，释放内存<br>删除后句柄失效，不能再使用<br>确保没有任务在使用该信号量 |

## ⚠️ 重要说明
**阻塞**：当计数值为0时，任务获取信号量会被挂起，直到有资源释放或超时

**资源管理**：计数值表示当前可用资源数量，获取时-1，释放时+1

**事件计数**：也可用于统计事件发生次数，每次事件发生释放信号量

<div align="center">

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
| **xSemaphoreCreateMutex**<br>`SemaphoreHandle_t xSemaphoreCreateMutex(void);` | 无 | ✅ 成功：有效句柄<br>❌ 失败：NULL | 创建互斥信号量<br>初始状态为 1（可用）<br>具有优先级继承特性 |
| **xSemaphoreTake**<br>`BaseType_t xSemaphoreTake(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xSemaphore`: 互斥量句柄<br>- `xTicksToWait`: 等待时间<br>  - `0`: 不等待<br>  - `portMAX_DELAY`: 无限等待<br>  - 具体数值: 等待指定tick数 | ✅ `pdTRUE`: 获取成功<br>❌ `pdFALSE`: 获取失败 | 获取互斥锁<br>互斥量=1 → 立即成功，状态1→0，任务成为持有者<br>互斥量=0 → 根据等待时间决定是否**阻塞**<br>不能在中断中使用 |
| **xSemaphoreGive**<br>`BaseType_t xSemaphoreGive(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 互斥量句柄 | ✅ `pdTRUE`: 释放成功<br>❌ `pdFALSE`: 释放失败 | 释放互斥锁<br>必须由持有者任务释放<br>互斥量=0 → 释放成功，状态0→1<br>互斥量=1 → 释放失败，状态不变 |
| **uxSemaphoreGetCount**<br>`UBaseType_t uxSemaphoreGetCount(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 互斥量句柄 | `0`: 被占用<br>`1`: 可用 | 获取互斥量当前状态<br>用于调试和状态检查<br>不影响互斥量状态 |
| **vSemaphoreDelete**<br>`void vSemaphoreDelete(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 要删除的互斥量句柄 | 无 | 删除互斥量，释放内存<br>删除后句柄失效，不能再使用<br>确保没有任务在使用该互斥量 |

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
| **xSemaphoreCreateRecursiveMutex**<br>`SemaphoreHandle_t xSemaphoreCreateRecursiveMutex(void);` | 无 | ✅ 成功：有效句柄<br>❌ 失败：NULL | 创建递归互斥量<br>初始状态为 1（可用）<br>内部计数初始为 0<br>具有优先级继承特性 |
| **xSemaphoreTakeRecursive**<br>`BaseType_t xSemaphoreTakeRecursive(`<br>`  SemaphoreHandle_t xSemaphore,`<br>`  TickType_t xTicksToWait`<br>`);` | - `xSemaphore`: 递归互斥量句柄<br>- `xTicksToWait`: 等待时间<br>  - `0`: 不等待<br>  - `portMAX_DELAY`: 无限等待<br>  - 具体数值: 等待指定tick数 | ✅ `pdTRUE`: 获取成功<br>❌ `pdFALSE`: 获取失败 | 递归获取互斥锁<br>首次获取：状态1→0，内部计数=1<br>重复获取：内部计数+1，不会**阻塞**<br>其他任务获取：会**阻塞**<br>不能在中断中使用 |
| **xSemaphoreGiveRecursive**<br>`BaseType_t xSemaphoreGiveRecursive(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 递归互斥量句柄 | ✅ `pdTRUE`: 释放成功<br>❌ `pdFALSE`: 释放失败 | 递归释放互斥锁<br>必须由持有者任务释放<br>部分释放：内部计数-1，状态保持0<br>完全释放：内部计数=0→状态0→1<br>必须与获取次数匹配 |
| **uxSemaphoreGetCount**<br>`UBaseType_t uxSemaphoreGetCount(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 递归互斥量句柄 | `0`: 被占用<br>`1`: 可用 | 获取递归互斥量当前状态<br>不反映内部计数<br>用于调试和状态检查 |
| **vSemaphoreDelete**<br>`void vSemaphoreDelete(`<br>`  SemaphoreHandle_t xSemaphore`<br>`);` | - `xSemaphore`: 要删除的递归互斥量句柄 | 无 | 删除递归互斥量，释放内存<br>删除后句柄失效，不能再使用<br>确保没有任务在使用该互斥量 |

## ⚠️ 重要说明
**阻塞**：当递归互斥量被其他任务持有时，任务获取互斥量会被挂起，直到互斥量释放或超时

**内部计数**：记录同一任务的获取次数，获取时+1，释放时-1，计数=0时真正释放

**匹配释放**：必须调用相同次数的GiveRecursive和TakeRecursive，否则会导致未定义行为

**优先级继承**：当高优先级任务等待递归互斥量时，系统会临时提升持有者任务的优先级

**中断限制**：递归互斥量不能在中断中使用（包括获取和释放）

<div align="center">

🎯 **适用于：递归函数、复杂调用链、面向对象设计** | 🚀 **防止自死锁**

