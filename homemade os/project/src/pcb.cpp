#include <dos.h>
#include "PCB.h"
#include "timer.h"
#include <iostream.h>


unsigned PCB::auto_id = 0;
volatile PCB* PCB::running = nullptr;
PCB* PCB::idlePCB = nullptr;
unsigned char PCB::globalIsSignalBlocked[NUM_OF_SIGNALS] = {0};

PCB::PCB(unsigned long int stack_size, unsigned int time_slice, void (*run_method)(), int isUserThread) {

	if (isUserThread) {

		stack = stack_size < MIN_STACK_SIZE ? new unsigned[MIN_STACK_SIZE] : new unsigned[stack_size];
		
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
	}
	this->stack_size = stack_size;
	this->time_slice = time_slice;
	if (time_slice == 0)
		status |= PCB_UNLIMITED_TIME_SLICE;
	else
		status = 0;
	id = auto_id++;
	blockedList = new PCBList();
	
	parent = (PCB*)running;
	if (parent != nullptr) {
		for (int i = 0; i < NUM_OF_SIGNALS; i++) {
			isSignalBlocked[i] = parent->isSignalBlocked[i];
			handlerListForSignal[i] = parent->handlerListForSignal[i];
		}
	}
	else {
		for (int i = 0; i < NUM_OF_SIGNALS; i++) {
			isSignalBlocked[i] = 0;
		}
		registerHandler(0, kill);
	}
}

PCB::~PCB() {
	delete stack;
	delete blockedList;
}

void PCB::unregisterAllHandlers(SignalId id) {
	if (id != 0) {
		handlerListForSignal[id].clearList();
	}
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	handlerListForSignal[id].swap(hand1, hand2);
}

void PCB::handleSignals() volatile {
	
	//cout << "Handlujemo signale\n";
	int signal = sigReqQueue.takeRequest();
	while (signal >= 0) {
		if (!globalIsSignalBlocked[signal] && !isSignalBlocked[signal] && signal != 0) {
			for (SignalHandler hand = handlerListForSignal[signal].begin(); 
			hand != handlerListForSignal[signal].end(); 
			hand = handlerListForSignal[signal].getNext()) {
				lockMacro;unlock;
					(*hand)();
				unlockMacro;lock;
			}
		}
		signal = sigReqQueue.takeRequest();
	}
	if (status & PCB_IS_KILLED) {
		lockMacro;
		//cout << "Ubijamo nit\n";
		kill();
		unlockMacro;
	}
}

void PCB::kill() {
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
	delete running->stack;
	running->stack = nullptr;
	delete running->blockedList;
	running->blockedList = nullptr;
	if (running->parent != nullptr)
		running->parent->signal(1);
	running->signal(2);
	running = Scheduler::get();
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
	lockMacro;
	if (running->parent != nullptr)
		running->parent->signal(1);
	running->signal(2);
	unlockMacro;
	exit_thread();
}

void PCB::idle_run() {
	cout << "idleThread running" << endl;
	while (1) {}
}

void PCB::init_running() {
	running = new PCB(4096, 2, nullptr, 0);
	running->status |= PCB_READY | PCB_STARTED;
	GlobalPCBList->insert((PCB*)running);
}

void PCB::init_Idle_PCB() {
	idlePCB = new PCB(4096, 1, idle_run);
	idlePCB->status |= PCB_READY | PCB_STARTED | PCB_IDLE_THREAD;
}
