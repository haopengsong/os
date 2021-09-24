// Pseudo-code for a multiprocessor queueing lock
// Both the scheduler and the lock use spinlocks to protect their internal data structures.
// Any thread that tries to acquire the lock when it is BUSY is put on a queue for later wakeup.
class Lock {
    private:
        int value = FREE;
        SpinLock spinLock;
        Queue waiting;
    public:
        void acquire();
        void release();
};

void
Lock::acquire() {
    spinLock.acquire();
    if (value != FREE ) {
        waiting.add( runningThread );
        scheduler.suspend( &spinLock );
        // scheduler releases spinLock
    } else {
        value = BUSY;
        spinLock.release();
    }
}

void 
Lock::release() {
    TCB *next;

    spinLock.acquire();
    if ( waiting.notEmpty() ) {
        next = waiting.remove();
        scheduler.makeReady( next );
    } else {
        value = FREE;
    }
    spinLock.release();
}

class Scheduler {
    private:
        Queue readyList;
        SpinLock schedulerSpinLock;
    public:
        void suspend(SpinLock *lock);
        void makeReady(Thread *thread);
};

void
Scheduler::suspend(SpinLock *lock) {
    TCB *chosenTCB;
    
    disableInterrupts();
    schedulerSpinLock.acquire();
    lock->release();
    runningThread->state = WAITING;
    chosenTCB = readyList.getNextThread();
    thread_switch(runningThread, chosenTCB);
    runningThread->state = RUNNING;
    schedulerSpinLock.release();
    enableInterrupts();

}

void
Scheduler::makeReady(TCB *thread) {
    disableInterrupts();
    schedulerSpinLock.acquire();
    readyList.add(thread);
    thread->state = READY;
    schedulerSpinLock.release();
    enableInterrupts();
}
















