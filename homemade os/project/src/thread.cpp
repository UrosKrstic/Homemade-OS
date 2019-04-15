#include "thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "timer.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
    lockMacro;
    myPCB = new PCB(stackSize, timeSlice);
    myPCB->myThread = this;
    unlockMacro;
}

void Thread::waitToComplete() {
    //mora se implentirati neki red za blokiranje
    //while (!myPCB->finished) {}
}

Thread::~Thread() {
    waitToComplete();
    delete myPCB;
}

void Thread::start() {
    lockMacro;
    if (!myPCB->thread_started) {
        myPCB->thread_started = 1;
        Scheduler::put(myPCB);
    }
    else {
        cout << "Thread " << myPCB->id << " already started!!!" << endl;
    }
    unlockMacro;
}

ID Thread::getRunningId() {
    return PCB::running->id;
}

