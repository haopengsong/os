// The interface and member variables for Readers/Writers Lock.
#ifndef RWLOCK_H
#define RWLOCK_H

class RWLock {
private:
    // Add synchronization variables by asking, "When can methods wait?"

    // First, add a mutual exclusion lock, 
    // the RWLock methods must wait whenever another thread is accessing the RWLock state variables.

    // Second, seems startRead and StartWrite may have to wait, 
    // so add a condition variable for each case: readerProceed, writerProceed.

    // Synchronization variables
    Lock lok;
    CV readerProceed;
    CV writerProceed;
    
    // State variables
    // For example, a call to startRead initially increments the number of waiting readers;
    // when the thread gets past the while loop, the number of waiting readers is decremented, but the 
    // number of active readers is incremented.
    int activeReaders;
    int activeWriters;
    int waitingReaders;
    int waitingWriters;

private:
    // predicate for the while loop ( startRead & startWrite may have to wait )
    // They are private methods and are always called from public methods that hold the mutual exclusion
    // lock, they do not need to acquire the lock.
    bool readShouldWait();
    bool writeShouldWait();

public:
    RWLock();
    ~RWLock();
    // doneRead and doneWrite do not wait (other than to acquire the mutual exclusion lock)
    // Therefore, these methods do not need condition variables
    // Always acquire/release lock at the beginning/end of a method (never in the middle)
    // We should write calls to acquire and release the mutual exclusion lock at the start and end of each public method before even thinking in detail about what these methods do.
    void startRead();
    void startWrite();
    // When reads or writes finish, it maybe possible for waiting threads to proceed
    // Therefore, need to call signal or broadcast to doneRead or doneWrite
    // When a read completes, there are two cases:
    // a) no writes are pending, and nothing needs to be done since this read cannot prevent other read
    // from proceeding, => Do nothing. 
    // Or
    // b) a write is pending, and this is the last active read, so one write can proceed, 
    // and any write waiting on the condition variable can proceed. => Use signal, since at most one write can
    // proceed and any write waiting on the CV can proceed.
    void doneRead();
    void doneWrite();

};
#endif
