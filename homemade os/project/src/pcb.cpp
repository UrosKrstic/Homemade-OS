#include <dos.h>
#include "PCB.h"
#include "timer.h"


unsigned PCB::auto_id = 0;
volatile PCB* PCB::running = 0;

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
	unlimited_time_slice = time_slice == 0 ? 1 : 0;
	finished = 0;
	id = auto_id++;
	thread_started = 0;
}

PCB::~PCB() {
	delete stack;
}

void PCB::exit_thread(){
	running->finished = 1;
	dispatch();
}  

void PCB::run_wrapper() {
	running->myThread->run();
	exit_thread();
}

void PCB::init_running() {
	running = new PCB();
}

PCB *p[3];