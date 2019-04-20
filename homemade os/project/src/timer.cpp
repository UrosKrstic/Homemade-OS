#include "timer.h"
#include "PCB.h"
#include <iostream.h>

//flag for disabling a context switch during critical instructions
//value 1 locks and disables context switching
//values 0 unlocks and enables context switching
volatile unsigned int lockFlag = 0;
//flag used to indicate demanded context switch
volatile int demanded_context_switch = 0;

//elapsed time in the program
unsigned long time_in_ticks = 0; // in ms

//helper variables for the timer interrupt
unsigned tsp;
unsigned tss;
unsigned tbp;

//old interrupt routine address
unsigned oldTimerOFF, oldTimerSEG;

//counter for context switching
volatile unsigned int counter = 20;

//interrupt routine
void interrupt tick() {
	if (!demanded_context_switch && !(PCB::running->status & PCB_UNLIMITED_TIME_SLICE))
		counter--;
	if (!demanded_context_switch)
		time_in_ticks += 55;
	if ((counter == 0 || demanded_context_switch) && !(PCB::running->status & PCB_UNLIMITED_TIME_SLICE)) {
		if (!lockFlag) {
			demanded_context_switch = 0;
			//stores stack and base pointers into the running PCB
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;
			//cout << "not fucked" << endl;
			//Scheduler used to get a new running PCB
			if (!(PCB::running->status & (PCB_BLOCKED | PCB_FINISHED | PCB_IDLE_THREAD))) {
				//if (PCB::running->id == 0)
					//cout << "fuck" << endl;
				Scheduler::put((PCB *)PCB::running);
			}
			PCB::running = Scheduler::get();
			
			
			//TODO: add Idle Thread
			if (PCB::running == nullptr) {
				//cout << "really fucked" << endl;
			}
			else {
				//cout << "New context: Thread with ID:" << PCB::running->id << endl;
			}

			//restores stack and base pointer from the new PCB
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

			//resets counter to the specified time slice
			//for the new running thread
			counter = PCB::running->time_slice;
		}
		else {
			demanded_context_switch = 1;
		}
	} 
	//calling the old timer interrupt routine
	//which had been moved to 60h
	//called only when no explicit context switch
	//is demanded - when an actual timer interrupt
	//has occurred
	if (!demanded_context_switch)
		asm int 60h;
}

// sets a new timer interrupt routine
void inic() {
	GlobalPCBList = new PCBList();
	PCB::init_running();
	//PCB::init_userMain();
	asm {
		cli
		push es
		push ax

		//initializes the timer routine
		mov ax,0
		mov es,ax

		//saves old routine
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax	
		mov ax, word ptr es:0020h	
		mov word ptr oldTimerOFF, ax

		//sets new routine
		mov word ptr es:0022h, seg tick
		mov word ptr es:0020h, offset tick

		//	sets old routine to a new position - entry 60h in IVT
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

// restores old timer interrupt routine
void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
	delete running;
	delete GlobalPCBList;
}


//synchronous context switch
void dispatch() {
	lock;
	demanded_context_switch = 1;
	tick();
	unlock;
}