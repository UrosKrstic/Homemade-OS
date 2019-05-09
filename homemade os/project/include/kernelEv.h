#ifndef _KERNELEV_H_
#define _KERNELEV_H_


class PCB;
class Event;
class IVTEntry;

class KernelEv {
    unsigned char ivtN;
    friend class Event;
    friend class IVTEntry;
    unsigned isWaitingForEvent;
    PCB* PCBOwner;
    KernelEv(unsigned char n);
    ~KernelEv();
    void signal();
    void wait();
};

#endif //_KERNELEV_H_