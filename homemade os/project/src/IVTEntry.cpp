#include "IVTEntry.h"
#include "PCB.h"
#include "kernelEv.h"
#include "timer.h"
#include <dos.h>

IVTEntry* IVT[NUMBER_OF_INTERRUPTS] = {nullptr};

IVTEntry::IVTEntry(unsigned char _n, unsigned _callOldIntr, pInterrupt intr) : 
    n(_n), callOldIntr(_callOldIntr) {
    lock;
    myEvent = nullptr;
    oldIntrRoutine = getvect(n);
    newIntrRoutine = intr;
    //setvect(n, intr);
    IVT[n] = this;
    unlock;
}

IVTEntry::~IVTEntry() {
    lock;
   //setvect(n, oldIntrRoutine);
    unlock;
}

void IVTEntry::signal() {
    lockMacro;
    if (callOldIntr) {
        (*oldIntrRoutine)();
    }
    if (myEvent != nullptr) {
        myEvent->signal();
    }
    unlockMacro;
}

void IVTEntry::deleteIVT() {
    for(int i = 0; i < NUMBER_OF_INTERRUPTS; i++) {
        if (IVT[i] != nullptr) {
            delete IVT[i];
        }
    }
}
