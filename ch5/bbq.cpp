/* 
 * A Blocking Bounded Queue:
 * 1) Thread trying to remove an item from an empty queue will wait until an item is available ;
 * 2) Thread trying to put an item into a full queue will wait until there is room ;
 */

// Thread-safe blocking queue
const int MAX = 10;

class BBQ {
    // Synchronization variables
    Lock lock;
    CV itemAdded;
    CV itemRemoved;
    
    // State variables
    int items[MAX];
    int front;
    int nextEmpty;

public:
    BBQ();
    ~BBQ() {}
    void insert(int item);
    int remove();
};

// Initialize the queue to empty
// the lock to free, and the condition variable to empty
BBQ::BBQ() {
    front = nextEmpty = 0;
}

// Wait until there is room and then insert an item.
void BBQ::insert(int item) {
    lock.acquire();
    while ((nextEmpty - front) == MAX) {
        itemRemoved.wait(&lock);
    }
    items[ nextEmpty % MAX ] = item;
    nextEmpty++;
    itemAdded.signal();
    lock.release();
}

// Wait until there is an item and 
// then remove an item
int BBQ::remove() {
    int item;
    lock.acquire();
    while (front == nextEmpty) {
        itemAdded.wait(&lock);
    }
    item = items[ front % MAX ];
    front++;
    itemRemved.signal();
    lock.release();
    return item;
}
