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
    IVTEntry(unsigned char _n, unsigned _callOldIntr, pInterrupt intr);
    ~IVTEntry();
    void signal();
    static void deleteIVT();
};

extern IVTEntry* IVT[NUMBER_OF_INTERRUPTS];


#define PREPAREENTRY(n, flag) void interrupt intr_##n(); \
    IV[##n] = new IVTEntry(##n, ##flag, intr_##n); \
    void interrupt intr_##n() { IV[##n]->signal(); }



#endif //_IVTENTRY_H_