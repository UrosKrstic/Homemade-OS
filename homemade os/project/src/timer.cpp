#include "timer.h"

//flag for disabling a context switch during critical instructions
//value 1 locks and disables context switching
//values 0 unlocks and enables context switching
volatile unsigned int lockFlag = 0;
//flag used to indicate demanded context switch
volatile int demanded_context_switch = 0;

extern volatile PCB* running;

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
	if (!demanded_context_switch && !running->unlimited_time_slice) counter--;
	if ((counter == 0 || demanded_context_switch) && !running->unlimited_time_slice) {
		if (!lockFlag) {
			demanded_context_switch = 0;
			//stores stack and base pointers into the running PCB
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;

			//Scheduler used to get a new running PCB
			if (!running->finished)
				Scheduler::put((PCB *)running);
			running = Scheduler::get();

			//restores stack and base pointer from the new PCB
			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

			//resets counter to the specified time slice
			//for the new running thread
			counter = running->time_slice;
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
}


//synchronous context switch
void dispatch() {
	asm cli;
	demanded_context_switch = 1;
	tick();
	asm sti;
}