/*
Synchronization barrier implementation details:
1) Create a Barrier class, with a lock to protect its internal state variables:
    - how many threads have checked in so far (count)
    - how many we are expecting (numThreads)
2) We acquire the lock at the begining of checkin, and we release it at the end;
3) We put the wait in a while loop, checking if all n threads have checked in yet;
4) The last thread to checkin does a broadcast to wake up all of the waiters;
*/

// A single use synchronization barrier
// With this implementation, each instance of a barrier can be safely used only one time.
class Barrier {
private:
    // synchronization variables
    Lock lok;
    CV allCheckedIn;

    // state variables
    int numEntered;
    int numThreads;

public:
        Barrier(int n);
        ~Barrier();
        void checkin();
};

Barrier::Barrier(int n) {
    numEntered = 0;
    numThreads = n;
}

// No one returns until all threads have call checkin
void checkin() {
    lok.acquire;
    numEntered++;
    if (numEntered < numThreads) {
        while (numEntered < numThreads) {
            allCheckedIn.wait( &lok );
        }
    } else { // last thread to checkin
       allCheckedIn.broadcast(); 
    }
    lok.release();
}
