// A reusable synchronization barrier
// 1) The first thread to leave (the one that wakes up the other threads) cannot reset the state,
// because until the other threads have woken up, the state is needed so that they know to exit the 
// while loop.
//
// 2) The last thread to leave the barrier cannot reset the state for the next iteration, because
// there is a possible race condition.
//
// How to implement?
//      - Use two single-use barriers.
//      - First barrier ensures that all threads are checked in.
//      - Second ensures that all threads have woken up from allCheckedIn.wait.
//      - The nth thread to leave can safely reset numCheckedIn; the nth thread to call checkin can 
//      safely reset numLeaving.
class Barrier {
private:
    // Synchronization variables
    Lock lok;
    CV allCheckedIn;
    CV allLeaving;

    // State variales
    int numEntered;
    int numLeaving;
    int numThreads;

public:
    Barrier(int n);
    ~Barrier();
    void checkin();
};

Barrier::Barrier(int n) {
    numEntered = 0;
    numLeaving = 0;
    numThreads = 0;
}

// No one returns until all threads
// have called checkin
void checkin() {
    lok.acquire();
    numEntered++;
    if (numEntered < numThreads) {
        while (numEntered < numThreads) {
            allCheckedIn.wait( &lok );
        }
    } else {
        // no threads in allLeaving.wait
        numLeaving = 0;
        allCheckedIn.broadcast();
    }
    numLeaving++;
    if (numLeaving < numThreads) {
        while (numLeaving < numThreads) {
            allLeaving.wait( &lok );
        }
    } else {
        // no threads in allCheckedIn.wait
        numEntered = 0;
        allLeaving.broadcast();
    }
    lok.release();
}
