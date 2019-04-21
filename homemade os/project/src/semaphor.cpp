#include "semaphor.h"
#include "kerSem.h"
#include "semList.h"
#include "timer.h"
#include <iostream.h>

Semaphore::Semaphore(int n) {
    lockMacro;
    myImpl = new KernelSem(n);
    GlobalSemaphoreList->insert(myImpl);
    cout << "Semaphore with id: " << myImpl->getID() << " created" << endl;
    unlockMacro;
}

Semaphore::~Semaphore() {
    lockMacro;
    GlobalSemaphoreList->remove(myImpl->getID());
    cout << "Semaphore with id: " << myImpl->id << " destroyed" << endl;
    delete myImpl;
    unlockMacro;
}

int Semaphore::wait(Time maxTimeToWait) {
    return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n) {
    return myImpl->signal(n);
}

int Semaphore::val() const {
    return myImpl->getValue();
}