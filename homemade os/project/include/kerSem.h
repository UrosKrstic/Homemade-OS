#ifndef _KERNEL_SEM_H_
#define _KERNEL_SEM_H_


class PCBList;
class WaitList;

class KernelSem {
    friend class Semaphore;
    friend class WaitList;
    friend class SemList;
    KernelSem(int value = 1);
    ~KernelSem();
    int wait(unsigned maxWaitTime);
    int signal(int n = 0);
    int getValue() const { return sem_value; }
    int getID() const { return id; }
    static unsigned auto_id;
    unsigned int id;
    int sem_value;
    PCBList *blockList;
    WaitList *sleepList;
};

#endif //_KERNEL_SEM_H_