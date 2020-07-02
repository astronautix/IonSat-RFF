#ifndef FREERTOS_H_INC
#define FREERTOS_H_INC

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "event/event.h"

namespace FFS {
    
    struct OSSettings {
        
    };

    template<typename evt_t>
    class Task {

        TaskHandle_t taskHandle;
        StaticTask_t task;
        StackType_t StackBuffer;
        FFS::Event<evt_t> event;

    public:

            // TASK CREATION : https://www.freertos.org/a00019.html
        Task(TaskFunction_t pxTaskCode, const char * const pcName, const uint32_t ulStackDepth, FFS::Event<evt_t> _event, UBaseType_t uxPriority):
            event{_event} {
            taskHandle = xTaskCreateStatic(pxTaskCode, pcName, ulStackDepth, reinterpret_cast<void*>(&event), uxPriority, &StackBuffer, &task);
        }

        ~Task() {
            vTaskDelete( taskHandle );
        }

            // TASK CONTROL : https://www.freertos.org/a00112.html
        static void delay(const TickType_t xTicksToDelay) {
            vTaskDelay( xTicksToDelay );
        }

        static void delayUntil(TickType_t *pxPreviousWakeTime, const TickType_t xTimeIncrement) {
            vTaskDelayUntil( pxPreviousWakeTime, xTimeIncrement );
        }

        UBaseType_t priority( ) {
            return uxTaskPriorityGet( taskHandle );
        }

        void priority( UBaseType_t uxNewPriority) {
            vTaskPrioritySet( taskHandle, uxNewPriority );
        }

        void suspend() {
            vTaskSuspend( taskHandle );
        }

        void resume() {
            vTaskResume( taskHandle );
        }

        BaseType_t resumeFromISR() {
            return xTaskResumeFromISR( taskHandle );
        }

        BaseType_t abortDelay() {
            return xTaskAbortDelay( taskHandle );
        }

            // TASK UTILITIES : https://www.freertos.org/a00021.html
        // TODO


    };


        // QUEUES : https://www.freertos.org/a00018.html
    class Queue {
        QueueHandle_t queueHandle;
        uint8_t storageBuffer;
        StaticQueue_t queue;

    public:
        Queue(UBaseType_t uxQueueLength,
              UBaseType_t uxItemSize,
              uint8_t *pucQueueStorageBuffer,
              StaticQueue_t *pxQueueBuffer) {

            queueHandle = xQueueCreateStatic(
                                         uxQueueLength,
                                         uxItemSize,
                                         &storageBuffer,
                                         &queue );

        }

        ~Queue() {
            vQueueDelete( queueHandle );
        }

        BaseType_t send(const void * pvItemToQueue, TickType_t xTicksToWait) {
            return  xQueueSend(queueHandle, pvItemToQueue, xTicksToWait);
        }

        BaseType_t sendFromISR(const void *pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
            return xQueueSendFromISR(queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
        }

        BaseType_t sendToBack( const void * pvItemToQueue, TickType_t xTicksToWait) {
            return xQueueSendToBack( queueHandle, pvItemToQueue, xTicksToWait);
        }

        BaseType_t sendToBackFromISR(const void *pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken){
            return xQueueSendToBackFromISR (queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
        }


        BaseType_t sendToFront(const void * pvItemToQueue, TickType_t xTicksToWait ) {
            return xQueueSendToFront( queueHandle, pvItemToQueue, xTicksToWait);
        }

        BaseType_t sendToFrontFromISR(const void *pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
            return xQueueSendToFrontFromISR ( queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
        }


        BaseType_t receive(void *pvBuffer, TickType_t xTicksToWait) {
            return xQueueReceive(queueHandle, pvBuffer, xTicksToWait);
        }

        BaseType_t receiveFromISR(void *pvBuffer, BaseType_t *pxHigherPriorityTaskWoken) {
            return xQueueReceiveFromISR(queueHandle, pvBuffer, pxHigherPriorityTaskWoken);
        }

        UBaseType_t messagesWaiting() {
            return uxQueueMessagesWaiting( queueHandle );
        }

        UBaseType_t messagesWaitingFromISR() {
            return uxQueueMessagesWaitingFromISR( queueHandle );
        }

        UBaseType_t spacesAvailable() {
            return uxQueueSpacesAvailable( queueHandle );
        }

        BaseType_t reset() {
            return xQueueReset( queueHandle );
        }

        BaseType_t isFullFromISR() {
            return xQueueIsQueueFullFromISR( queueHandle );
        }

        BaseType_t isEmptyFromISR() {
            return xQueueIsQueueEmptyFromISR( queueHandle );
        }

        BaseType_t overwrite(const void * pvItemToQueue) {
            return xQueueOverwrite( queueHandle, pvItemToQueue );
        }

        BaseType_t overwriteFromISR(const void * pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
            return xQueueOverwriteFromISR( queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
        }


        BaseType_t peek(void *pvBuffer, TickType_t xTicksToWait) {
            return xQueuePeek( queueHandle, pvBuffer, xTicksToWait );
        }

        BaseType_t peekFromISR(void *pvBuffer) {
            return xQueuePeekFromISR(queueHandle, pvBuffer);
        }


        /*
        void addToRegistry(char *pcQueueName) {
            vQueueAddToRegistry( queueHandle, pcQueueName);
        }


        void unregister() {
            vQueueUnregisterQueue( queueHandle );
        }

        const char *getName() {
            return pcQueueGetName( queueHandle );
        }
        */




    };

    class Semaphore {
        SemaphoreHandle_t semaphoreHandle;
        StaticSemaphore_t semaphoreBuffer;

    public:
        Semaphore() {
            semaphoreHandle = xSemaphoreCreateBinaryStatic(&semaphoreBuffer);
        }

        Semaphore(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount) {
            semaphoreHandle = xSemaphoreCreateCountingStatic(uxMaxCount, uxInitialCount, &semaphoreBuffer);
        }

        ~Semaphore() {
            vSemaphoreDelete( semaphoreHandle );
        }

    };



}

extern "C" {
    void vMainQueueSendPassed( void )
    {
        /* This is just an example implementation of the "queue send" trace hook. */

    }

    void vApplicationIdleHook( void )
    {

        #ifdef __GCC_POSIX__
            struct timespec xTimeToSleep, xTimeSlept;
                /* Makes the process more agreeable when using the Posix simulator. */
                xTimeToSleep.tv_sec = 1;
                xTimeToSleep.tv_nsec = 0;
                nanosleep( &xTimeToSleep, &xTimeSlept );
        #endif
    }

    
// From https://www.freertos.org/a00110.html
/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static – otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task’s
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task’s stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*———————————————————–*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
    /* If the buffers to be provided to the Timer task are declared inside this
    function then they must be declared static – otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task’s state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task’s stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


    // https://www.freertos.org/pvTaskGetThreadLocalStoragePointer.html
    void* pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery, BaseType_t xIndex ){
        // TODO
    }

    void vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,  BaseType_t xIndex, void *pvValue ){
        // TODO
    }

    TaskHandle_t xTaskGetIdleTaskHandle() {

    }

}


#endif
