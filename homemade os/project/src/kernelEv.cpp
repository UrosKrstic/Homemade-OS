#include "kernelEv.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "IVTEntry.h"
#include "timer.h"
#include <dos.h>

KernelEv::KernelEv(unsigned char n) : 
    ivtN(n), isWaitingForEvent(0), PCBOwner((PCB*)PCB::running) {
    if (IVT[ivtN] != nullptr) {
        IVT[ivtN]->myEvent = this;
        setvect(ivtN, IVT[ivtN]->newIntrRoutine);
    }
}

KernelEv::~KernelEv() {
    if (isWaitingForEvent) {
        PCBOwner->status |= PCB_READY;
        PCBOwner->status &= ~PCB_BLOCKED;
        Scheduler::put(PCBOwner);
    }
    if (IVT[ivtN] != nullptr) {
        IVT[ivtN]->myEvent = nullptr;
        setvect(ivtN, IVT[ivtN]->oldIntrRoutine);
    }
}

void KernelEv::wait() {
    lockMacro;
    if (PCBOwner == PCB::running) {
        if (!isWaitingForEvent) {
            isWaitingForEvent = 1;
            PCBOwner->status |= PCB_BLOCKED;
            PCBOwner->status &= ~PCB_READY;
            dispatch();
        }
    }
    unlockMacro;
}

void KernelEv::signal() {
    lockMacro;
    if (isWaitingForEvent) {
        isWaitingForEvent = 0;
        PCBOwner->status |= PCB_READY;
        PCBOwner->status &= ~PCB_BLOCKED;
        Scheduler::put(PCBOwner);
        dispatch();
    }
    unlockMacro;
}
