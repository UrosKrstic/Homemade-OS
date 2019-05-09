#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_
#define NUMBER_OF_INTERRUPTS 256

class KernelEv;

typedef void interrupt (*pInterrupt) (...);

class IVTEntry {
    friend KernelEv;
    unsigned char n;
    unsigned callOldIntr;
    KernelEv* myEvent;
public:
    pInterrupt oldIntrRoutine;
    pInterrupt newIntrRoutine;
    IVTEntry(unsigned char _n, unsigned _callOldIntr, pInterrupt intr);
    ~IVTEntry();
    void signal();
    static void deleteIVT();
};

extern IVTEntry* IVT[NUMBER_OF_INTERRUPTS];

#define PREPAREENTRY(entry, flag) \
    void interrupt intr_##entry(...); \
    IVTEntry ivt##entry(entry, flag, intr_##entry); \
    void interrupt intr_##entry(...) { \
        ivt##entry.signal(); \
    }

#endif //_IVTENTRY_H_