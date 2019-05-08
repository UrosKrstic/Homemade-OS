#include <dos.h>
#include "PCB.h"
#include "timer.h"
#include <iostream.h>


unsigned PCB::auto_id = 0;
volatile PCB* PCB::running = nullptr;
PCB* PCB::idlePCB = nullptr;

PCB::PCB(unsigned long int stack_size, unsigned int time_slice, void (*run_method)()) {
	unsigned* stack = new unsigned[stack_size];
	
	//sets the I flag in the PSW register
	//of the new thread
	stack[stack_size - PSW_OFFSET] = PSW_INIT_VAL;

	//sets the starting PC of the thread
	stack[stack_size - RUN_METHOD_SEG_OFFSET] = FP_SEG(run_method);
	stack[stack_size - RUN_METHOD_OFF_OFFSET] = FP_OFF(run_method);

	//"pushing all registers on the stack"
	sp = FP_OFF(stack + stack_size - ALL_REGISTERS_OFFSET);
	ss = FP_SEG(stack + stack_size - ALL_REGISTERS_OFFSET);
	bp = FP_OFF(stack + stack_size - ALL_REGISTERS_OFFSET);

	this->stack_size = stack_size;
	this->time_slice = time_slice;
	if (time_slice == 0)
		status |= PCB_UNLIMITED_TIME_SLICE;
	else
		status = 0;
	id = auto_id++;
	blockedList = new PCBList();
}


PCB::~PCB() {
	delete stack;
	delete blockedList;
}

void PCB::exit_thread(){
	lockMacro;
	running->status = PCB_FINISHED;
	while (1) {
		PCB* pcb = running->blockedList->remove();
		if (pcb != nullptr) {
			pcb->status &= ~PCB_BLOCKED;
			pcb->status |= PCB_READY;
			Scheduler::put(pcb);
		}
		else {
			break;
		}
	}
	dispatch();
	unlockMacro;
}  

void PCB::run_wrapper() {
	running->myThread->run();
	exit_thread();
}

void PCB::idle_run() {
	cout << "idleThread running" << endl;
	while (1) {}
}

void PCB::init_running() {
	running = new PCB(4096, 20);
	running->status |= PCB_READY | PCB_STARTED;
	GlobalPCBList->insert((PCB*)running);
}

void PCB::init_Idle_PCB() {
	idlePCB = new PCB(4096, 1, idle_run);
	idlePCB->status |= PCB_READY | PCB_STARTED | PCB_IDLE_THREAD;
}
