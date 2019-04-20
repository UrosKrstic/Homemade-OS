#ifndef _PCB_H_
#define _PCB_H_
#include <dos.h>
#include "thread.h"
#include "PCBList.h"

//Initializes I bit in PSW to 1
//all other bits to 0
#define PSW_INIT_VAL 0x200

//offsets on the stack 
#define PSW_OFFSET 1
#define RUN_METHOD_SEG_OFFSET 2
#define RUN_METHOD_OFF_OFFSET 3
#define ALL_REGISTERS_OFFSET 12

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


class PCBList;

//Process Control Block (PCB) used to save the context of the running thread
//and to initialize a new context
//used during context switching
class PCB {
private:
	static volatile PCB* running;
	friend void interrupt tick();
	friend void restore();
	friend class Thread;
	static unsigned auto_id;
	unsigned id;
	unsigned long int stack_size;
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned status;
	unsigned int time_slice;
	PCB(unsigned long int stack_size = 4096, unsigned int time_slice = 2, void (*run_method)() = run_wrapper);
	~PCB();
	static void exit_thread();	
	static void run_wrapper();
	Thread *myThread;
	PCBList* blockedList;
public:
	static void init_running();
	static void init_userMain();
	unsigned getID() { return id; }
};

extern PCBList* GlobalPCBList;

#endif //_PCB_H_