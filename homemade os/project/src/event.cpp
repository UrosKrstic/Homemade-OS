#include "event.h"
#include "kernelEv.h"
#include "timer.h"

Event::Event(IVTNo ivtNo) {
    lock;
    myImpl = new KernelEv(ivtNo);
    unlock;
}

Event::~Event() {
    lock;
    delete myImpl;
    unlock;
}

void Event::wait() {
    myImpl->wait();
}

void Event::signal() {
    myImpl->signal();
}
