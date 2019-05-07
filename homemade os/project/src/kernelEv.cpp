#include "kernelEv.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "IVTEntry.h"
#include "timer.h"

KernelEv::KernelEv(unsigned char n) : ivtN(n) {
    PCBOwner = (PCB*)PCB::running;
    IVT[ivtN]->myEvent = this;
}

KernelEv::~KernelEv() {
    if (PCBOwner->status & PCB_BLOCKED) {
        PCBOwner->status |= PCB_READY;
        PCBOwner->status &= ~PCB_BLOCKED;
        Scheduler::put(PCBOwner);
    }
}

void KernelEv::wait() {
    lockMacro;
    if (PCBOwner == PCB::running) {
       PCBOwner->status |= PCB_BLOCKED;
       PCBOwner->status &= ~PCB_READY;
    }
    unlockMacro;
    dispatch();
}

void KernelEv::signal() {
    lockMacro;
    if (PCBOwner->status & PCB_BLOCKED) {
        PCBOwner->status |= PCB_READY;
        PCBOwner->status &= ~PCB_BLOCKED;
        Scheduler::put(PCBOwner);
    }
    unlockMacro;
    dispatch();
}
