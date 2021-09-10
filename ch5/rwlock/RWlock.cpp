#include "RWlock.h"

// Wait until no active or waiting writes, then proceed
void RWLock::startRead() {
    lok.acquire();
    waitingReaders++; 
    while ( readShouldWait() ) {
        readerProceed.Wait(&lock);
    }
    waitingReaders--;
    activeReaders++;
    lok.release();
}

// Done reading. If no other active reads, a 
// write may proceed
void RWLock::doneRead() {
    lok.acquire();
    activeReaders--;
    if ( activeReaders == 0 && waitingWriters > 0 ) {
        writerProceed.signal();
    }
    lok.release();
}

// Wait until no active read or
// write then proceed
void RWLock::startWrite() {
    lok.acquire();
    waitingWriters++;
    while ( writeShouldWait() ) {
        writerProceed.Wait( &lock );
    }
    waitingWriters--;
    activeWriters++;
    lok.release(); 
}

// If there are any pending writes, 
// we signal on writersProceed. Otherwise, we broadcast on readersProceed.
// Done writing. A waiting write or read may proceed.
void RWLock::doneWrite() {
   lok.acquire();
   activeWriters--; 
   assert( activeWriters == 0 );
   if ( waitingWriters > 0 ) {
        writerProceed.signal();
   } else {
       readersProceed.broadcast(); 
   }
   lok.release();
}

// Define predicates
// A writers preferred solution:
// 1) Reads should wait if there are any active or pending writers;
// 2) Writes wait only while there are active readers or active writers;
// Otherwise, a continuous stream of new readers could starve a write request and prevent it from 
// ever being serviced.

bool RWLock::readShouldWait() {
    return ( activeWriters > 0 || waitingWriters > 0 );
}

bool RWLock::writeShouldWait() {
    return ( activeReaders > 0 || activeWriters > 0 );
}

