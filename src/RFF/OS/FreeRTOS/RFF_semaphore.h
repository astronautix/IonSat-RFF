#pragma once

namespace RFF {

	/**
		\brief Synchronization primitive for concurrent ressource access
	*/
	class Semaphore {
	protected:
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
			vSemaphoreDelete(semaphoreHandle);
		}
		
		SemaphoreHandle_t const& handle() const {
            return semaphoreHandle;
        }

		UBaseType_t getCount() {
			return uxSemaphoreGetCount(semaphoreHandle);
		}

		bool take(TickType_t ticksToWait = 0) {
			return xSemaphoreTake(semaphoreHandle, ticksToWait);
		}

		bool give() {
			return xSemaphoreGive(semaphoreHandle);
		}

		// TODO
		/*
		bool takeFromISR( TickType_t ticksToWait = 0 ) {
		    return xSemaphoreTakeFromISR( semaphoreHandle, ticksToWait );
		}
		*/



	};

	/**
		\brief Synchronization primitive for mutual exclusion (Special case of Semaphore)

		
	*/
	class Mutex : public Semaphore {
	public:
		Mutex() {
			semaphoreHandle = xSemaphoreCreateMutexStatic(&semaphoreBuffer);
		}
	};
    
	/**
		\brief RAII container for holding Semaphores
	*/
    class SemaphoreHolder {
        Semaphore* ref;
    public:
        
        SemaphoreHolder() : ref{} {};
        
        SemaphoreHolder(Semaphore& _ref) : ref{&_ref} {
            ref->take();
        };
        
        ~SemaphoreHolder() {
            if(ref) {
                ref->give();
            }
        }
        
        SemaphoreHolder(SemaphoreHolder const& other) = delete;
        SemaphoreHolder& operator=(SemaphoreHolder const& other) = delete;
        
        SemaphoreHolder(SemaphoreHolder&& other) : ref{other.ref}{ 
            if(other.ref) { other.ref->give(); }
            other.ref = nullptr; 
            
        };
        SemaphoreHolder& operator=(SemaphoreHolder&& other) {
            if(ref) { ref->give(); } 
            ref = other.ref; 
            other.ref = nullptr; 
            
            return *this;
        };
    };
}
