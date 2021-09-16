// A FIFO blocking bounded buffer, technically pseudo code. 
// ConditionQueue is a linkedList of condition variables

// FIFO constraint: the queue is FIFO. The nth thread to acquire the lock in remove
// retrieves the item inserted by the nth thread to acquire the lock in insert.
// When one thread signals a waiter, the waiting thread (and not any other) removes 
// the item.
ConditionQueue insertQueue;
ConditionQueue removeQueue;
int numRemoveCalled = 0; // # of times remove has been called
int numInsertedCalled = 0; // # of times insert has been called

int 
FIFOBBQ::remove() {
    int item;
    int myPosition;
    CV *myCV, *nextWaiter;
    lok.acquire();

    myPosition = numRemoveCalled++;
    myCV = new CV(); // Create a new condition variable to wait on 
    removeQueue.append( myCV )
    // Even if I am woken up, wait until it is my turn
    while (front < myPosition || front == nextEmpty) {
        myCV->wait( &lok );
    }
    delete self; // The condition variable is no longer needed
    item = items[front % size];
    front++;

    // Wake up the next thread waiting in insert, if any.
    nextWaiter = insertQueue.removeFromFront();
    if (nextWaiter != nullptr) {
        nextWaiter->signal( &lok );
    }
    lok.release();
    return item;
}
