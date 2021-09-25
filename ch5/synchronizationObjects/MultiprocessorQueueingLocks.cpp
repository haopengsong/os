// Pseudo-code for a multiprocessor queueing lock
// Both the scheduler and the lock use spinlocks to protect their internal data structures.
// Any thread that tries to acquire the lock when it is BUSY is put on a queue for later wakeup.
class Lock {
    private:
        int value = FREE;
        // To reduce contention on the ready list spinlock, we use a {separate} spinlock to guard 
        // access to teach lock's internal state
        SpinLock spinLock;
        Queue waiting;
    public:
        void acquire();
        void release();
};

void
Lock::acquire() {
    spinLock.acquire();
    // Once a thread holds the lock's spinlock, the thread can inspect and update the lock's state
    // 1) If the lock is FREE, the thread sets the value and releases its spinlock;
    // 2) If the lock is BUSY, more work is needed:
    //  - Need to put the current thread on the waiting list for the lock;
    //  - Suspend the current running thread; see line 64 
    //  - And switch to a new thread => done in the scheduler side;
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

// The scheduler ready list needs a spinlock. The scheduler holds this spinlock for 
// only a few instructions
// More: 
// 1) If the ready list spinlock is BUSY, there is no point in trying to switch to a different
// thread, as that would require access to the ready list

class Scheduler {
    private:
        Queue readyList;
        SpinLock schedulerSpinLock;
    public:
        void suspend(SpinLock *lock);
        void makeReady(Thread *thread);
};

// Need careful sequencing to suspend a thread on a multiprocessor:
// **********************************
// 1) First disableInterrupts() to ensure thread is not preempted while holding the ready list spinlock
// 2) Acquire the ready list spinlock, and {only then} is it safe to release the lock's spinlock and
//    switch to a new thread;
// 3) The ready list spinlock is released by the next thread to run; 
// **********************************
// Otherwise, a different thread on another processor might put the waiting thread back
// on the ready list (and start it running) before the waiting thread has completed its context switch

// Later, when the lock is release, if any threads are waiting for the lock, one of them is 
// moved off the lock's waitiing list to the scheduler's ready list 
// =>  Lock::release => Scheduler::makeReady
void
Scheduler::suspend(SpinLock *lock) {
    TCB *chosenTCB;
    
    disableInterrupts();
    schedulerSpinLock.acquire(); // To guard the readyList 
                                 // from being accessed by thread from other processor
    lock->release();
    runningThread->state = WAITING;
    chosenTCB = readyList.getNextThread();
    thread_switch(runningThread, chosenTCB);
    runningThread->state = RUNNING;
    // The new thread will reschedule itself
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
















