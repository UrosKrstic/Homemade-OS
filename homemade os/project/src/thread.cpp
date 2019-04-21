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
        while (!(myPCB->status & PCB_FINISHED)) {
            PCB::running->status |= PCB_BLOCKED;
            PCB::running->status &= ~PCB_READY; 
            myPCB->blockedList->insert((PCB*)PCB::running);
            dispatch();
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
