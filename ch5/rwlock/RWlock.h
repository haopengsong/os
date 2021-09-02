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
    int activeReaders;
    int activeWriters;
    int waitingReaders;
    int waitingWriters;

private:
    bool readShouldWait();
    bool writeShouldWait();

public:
    RWLock();
    ~RWLock();
    void startRead();
    void doneRead();
    void startWrite();
    void doneWrite();

};
#endif
