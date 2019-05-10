#ifndef _PCB_H_
#define _PCB_H_
#include <dos.h>
#include "thread.h"
#include "PCBList.h"
#include "SigReqQ.h"
#include "HandList.h"

//Initializes I bit in PSW to 1
//all other bits to 0
#define PSW_INIT_VAL 0x200

//offsets on the stack 
#define PSW_OFFSET 1
#define RUN_METHOD_SEG_OFFSET 2
#define RUN_METHOD_OFF_OFFSET 3
#define ALL_REGISTERS_OFFSET 12
#define NUM_OF_SIGNALS 16

//definition, looks tidier
//better than writing 0 
#define nullptr (void*)0

//STATUS BITS
#define PCB_READY 1
#define PCB_BLOCKED 2
#define PCB_FINISHED 4
#define PCB_STARTED 8
#define PCB_UNLIMITED_TIME_SLICE 16
#define PCB_IDLE_THREAD 32
#define PCB_TIMEOUT_DEBLOCK 64
#define PCB_LOCK 128

class PCBList;
class KernelEv;

//Process Control Block (PCB) used to save the context of the running thread
//and to initialize a new context
//used during context switching
class PCB {
private:
	static volatile PCB* running;
	static PCB* idlePCB;
	friend void interrupt tick();
	friend void restore();
	friend void inic();
	friend void dispatch();
	friend class Thread;
	friend class KernelSem;
	friend class SemList;
	friend class IVTEntry;
	friend class KernelEv;
	static unsigned auto_id;
	unsigned id;
	unsigned long int stack_size;
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned status;
	unsigned int time_slice;

	PCB* parent;
	SignalRequestQueue sigReqQueue;
	unsigned char isSignalBlocked[NUM_OF_SIGNALS];
	SignalHandlerList handlerListForSignal[NUM_OF_SIGNALS];

	static unsigned char globalIsSignalBlocked[NUM_OF_SIGNALS];

	void saveSignalRequest(SignalId) {
		sigReqQueue.saveRequest(signal);
	}
	void registerHandler(SignalId signal, SignalHandler handler) {
		handlerListForSignal[signal].insertHandler(handler);
	}
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void handleSignals() volatile;
	

	PCB(unsigned long int stack_size = 4096, unsigned int time_slice = 2, void (*run_method)() = run_wrapper);
	~PCB();
	static void exit_thread();	
	static void run_wrapper();
	static void idle_run();
	static void init_running();
	static void init_Idle_PCB();
	Thread *myThread;
	PCBList* blockedList;
public:
	unsigned getID() const volatile { return id; }
};

#endif //_PCB_H_