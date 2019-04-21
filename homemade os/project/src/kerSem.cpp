#include "kerSem.h"
#include "PCBList.h"
#include "waitList.h"
#include "timer.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include <iostream.h>

unsigned KernelSem::auto_id = 0;

KernelSem::KernelSem(int value) {
    sem_value = value;
    id = auto_id++;
    blockList = new PCBList();
    sleepList = new WaitList();
}   

KernelSem::~KernelSem() {
    
    PCB* pcb =  blockList->remove();
    while (pcb != nullptr) {
        pcb->status &= ~PCB_BLOCKED;
        pcb->status |= PCB_READY;
        Scheduler::put(pcb);
        pcb =  blockList->remove();
    }
    pcb =  sleepList->remove();
    while (pcb != nullptr) {
        pcb->status &= ~PCB_BLOCKED;
        pcb->status |= PCB_READY;
        Scheduler::put(pcb);
        pcb =  sleepList->remove();
    }
    delete blockList;
    delete sleepList;
}

int KernelSem::wait(unsigned maxWaitTime) {
    lockMacro;
    cout << "sem value:" << sem_value << endl;
    if (--sem_value < 0) {
        cout << "gonna get blocked: " << PCB::running->getID() << endl; 
        cout << "sem value:" << sem_value << endl;
        PCB::running->status &= ~PCB_READY;
        PCB::running->status |= PCB_BLOCKED;
        if (maxWaitTime > 0) {
            sleepList->insert((PCB*)PCB::running, maxWaitTime);
        }
        else {
            blockList->insert((PCB*)PCB::running);
        }
        dispatch();
    }
    unlockMacro;
    return !(PCB::running->status & PCB_TIMEOUT_DEBLOCK);
}

int KernelSem::signal(int n) {
    lockMacro;
    if (n == 0) {
        if (sem_value++ < 0) {
            cout << "sem value:" << sem_value << endl;
            cout << "unblocking a thread" << endl;
            PCB* pcb = blockList->remove();
            if (pcb == nullptr) {
                pcb = sleepList->remove();
            }
            if (pcb != nullptr) {
                pcb->status &= ~PCB_BLOCKED;
                pcb->status |= PCB_READY;
                pcb->status &= ~PCB_TIMEOUT_DEBLOCK;
                Scheduler::put(pcb);
            }
        }
        unlockMacro;
        return 0;
    }
    else if (n > 0) {
        sem_value += n;
        int unblocked_cnt = 0;
        cout << "sem value:" << sem_value << endl;
            cout << "unblocking threads" << endl;
        while (n > 0) {
            PCB* pcb = blockList->remove();
            if (pcb == nullptr)
                break;
            else {
                pcb->status &= ~PCB_BLOCKED;
                pcb->status |= PCB_READY;
                pcb->status &= ~PCB_TIMEOUT_DEBLOCK;
                Scheduler::put(pcb);
                unblocked_cnt++;
                n--;
            }
        }
        while (n > 0) {
             PCB* pcb = sleepList->remove();
            if (pcb == nullptr)
                break;
            else {
                pcb->status &= ~PCB_BLOCKED;
                pcb->status |= PCB_READY;
                pcb->status &= ~PCB_TIMEOUT_DEBLOCK;
                Scheduler::put(pcb);
                unblocked_cnt++;
                n--;
            }
        }
        unlockMacro;
        return unblocked_cnt;
    }
    else {
        unlockMacro;
        return n;
    } 
}
