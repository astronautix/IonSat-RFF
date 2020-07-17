#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <boost/container/static_vector.hpp>
#include <algorithm>

namespace FFS {
    template<typename evt_t, uint32_t stackDepth>
    class Task {

    public:

        TaskHandle_t taskHandle;
        StaticTask_t task;
        StackType_t stackBuffer[stackDepth];
        FFS::Event<evt_t> event;
        void (*const handler)(void*);
        std::string name;


        // NO COPY
        Task(Task const& other) = delete;
        Task& operator= (Task const& other) = delete;

        // MOVE ALLOWED
        Task(Task<evt_t, stackDepth>&& other) : task{std::move(other.task) }, 
                                                stackBuffer{std::move(* (other.stackBuffer)) },  
                                                event{std::move(other.event) }, 
                                                handler{other.handler},
                                                name{other.name} {
            taskHandle = std::move(other.taskHandle);
            other.taskHandle = 0;
        }
        
        Task& operator= (Task&& other) {
            task = std::move(other.task);
            //memcpy(stackBuffer, other.stackBuffer, sizeof(other.stackBuffer));
            std::copy(std::begin(other.stackBuffer), std::end(other.stackBuffer), std::begin(stackBuffer));
            taskHandle = std::move(other.taskHandle);
            name = std::move(other.name);
            other.taskHandle = 0;
            return *this;
        }

        // TASK CREATION : https://www.freertos.org/a00019.html
        Task(void (*const _handler)(void*), std::string _name, FFS::Event<evt_t> _event, UBaseType_t uxPriority) :
            event{_event}, handler{_handler}, name{_name} {
                
            taskHandle = xTaskCreateStatic(handler, name.c_str(), stackDepth, &event, uxPriority, stackBuffer, &task);
            assert(taskHandle != 0);
        }
        
        Task(void (*const _handler)(void*), std::string _name, UBaseType_t uxPriority) :
            event{}, handler{_handler}, name{_name} {
                
            taskHandle = xTaskCreateStatic(handler, name.c_str(), stackDepth, &event, uxPriority, stackBuffer, &task);
            assert(taskHandle != 0);
        }

        ~Task() {
            if(taskHandle != 0) { // do not delete if moved from
                vTaskDelete(taskHandle);
            }
        }

        // TASK CONTROL : https://www.freertos.org/a00112.html
        static void delay(const TickType_t xTicksToDelay) {
            vTaskDelay(xTicksToDelay);
        }

        static void delayUntil(TickType_t* pxPreviousWakeTime, const TickType_t xTimeIncrement) {
            vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
        }

        UBaseType_t priority() {
            return uxTaskPriorityGet(taskHandle);
        }

        void priority(UBaseType_t uxNewPriority) {
            vTaskPrioritySet(taskHandle, uxNewPriority);
        }

        void suspend() {
            vTaskSuspend(taskHandle);
        }
        
        static void suspendCurrent() {
            vTaskSuspend(NULL);
        }

        void resume() {
            vTaskResume(taskHandle);
        }

        BaseType_t resumeFromISR() {
            return xTaskResumeFromISR(taskHandle);
        }

        BaseType_t abortDelay() {
            return xTaskAbortDelay(taskHandle);
        }

        // TASK UTILITIES : https://www.freertos.org/a00021.html
        // TODO


    };
}
