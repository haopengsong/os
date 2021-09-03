#include "RWlock.h"

void RWLock::startRead() {
    lok.acquire();

    lok.release();
}

void RWLock::doneRead() {
    lok.acquire();

    lok.release();
}
