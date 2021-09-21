/*
Pseudo-code for a uniprocessor queueing lock.
If a lock is BUSY when a thread tries to acquire it, the thread moves its TCB onto the lock's waiting list.
The thread then suspends itself and switches to the next runnable thread. 
The call to suspend does not return until the thread is put back on the ready list, e.g., until some thread calls
Lock::release;
*/
class Lock {
private:
    int value = FREE;
    Queue waiting;
public:
    void acquire();
    void release();
};

Lock::acquire() {
    TCB *chosenTCB;
    
    disableInterrupts();
    if (value == BUSY) {
        waiting.add(runningThread);
        runningThread->state = WAITING;
        chosenTCB = readyList.remove();
        thread_switch(runningThread, chosenTCB);
        runningThread->state = RUNNING;
    } else {
        value = BUSY;
    }
    enableInterrupts();
}

Lock::release() {
    // Next thread to hold lock
    TCB *next;

    disableInterrupts();
    if ( waiting.notEmpty() ) {
        // move one TCB from waiting to ready
        next = waiting.remove();
        next->state = READY; 
        readyList.add( next );
    } else {
        value = FREE;
    }
    enableInterrupts();
}
/*
If a thread is waiting for the lock, a call to release does not set value to FREE. 
Instead, it leaves value as BUSY. The woken thread is guaranteed to be the next that executes the 
critical section. THis arrangement ensures freedom from starvation.
*/
