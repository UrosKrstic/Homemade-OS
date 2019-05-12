#include "thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "timer.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
    lockMacro;
    myPCB = new PCB(stackSize, timeSlice);
    myPCB->myThread = this;
    GlobalPCBList->insert(myPCB);
    unlockMacro;
}

void Thread::waitToComplete() {
    lockMacro;
    if (myPCB->getID() != getRunningId()) {
        while (!(myPCB->status & PCB_FINISHED) && (myPCB->status & PCB_STARTED)) {
            PCB::running->status |= PCB_BLOCKED;
            PCB::running->status &= ~PCB_READY; 
            myPCB->blockedList->insert((PCB*)PCB::running);
            lockFlag = 0;
            dispatch();
            lockFlag = 1;
        }
    }
    unlockMacro;
}

Thread::~Thread() {
    waitToComplete();
    lockMacro;
    GlobalPCBList->remove(myPCB->getID());
    delete myPCB;
    unlockMacro;
}

void Thread::start() {
    lockMacro;
    if (!(myPCB->status & PCB_STARTED)) {
        myPCB->status |= PCB_STARTED;
        Scheduler::put(myPCB);
    }
    unlockMacro;
}

ID Thread::getRunningId() {
    return PCB::running->id;
}

Thread* Thread::getThreadById(ID id) {
    PCB* pcb = GlobalPCBList->searchById(id);
    if (pcb != nullptr)
        return pcb->myThread;
    else
        return nullptr;
}

ID Thread::getId() { return (int)myPCB->id; };

void Thread::signal(SignalId signal) {
    if (signal == 0) myPCB->status |= PCB_IS_KILLED;
    myPCB->saveSignalRequest(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
    myPCB->registerHandler(signal, handler);
}

void Thread::unregisterAllHandlers(SignalId id) {
    myPCB->unregisterAllHandlers(id);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
    myPCB->swap(id, hand1, hand2);
}

void Thread::blockSignal(SignalId signal) {
    myPCB->isSignalBlocked[signal] = 1;
}

void Thread::blockSignalGlobally(SignalId signal) {
    PCB::globalIsSignalBlocked[signal] = 1;
}

void Thread::unblockSignal(SignalId signal) {
     myPCB->isSignalBlocked[signal] = 0;
}

void Thread::unblockSignalGlobally(SignalId signal) {
     PCB::globalIsSignalBlocked[signal] = 0;
}
