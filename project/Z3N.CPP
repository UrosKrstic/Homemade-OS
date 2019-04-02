//memory model in use: huge
//simple kernel implementation

#include <iostream.h>
#include <dos.h>
#include "schedule.h"

//used for disabling interrupts
//and not allowing context switching
#define lock asm cli

//used for enabling interrupts
//opposite of lock
#define unlock asm sti

//flag for disabling a context switch during critical instructions
//value 1 locks and disables context switching
//values 0 unlocks and enables context switching
volatile unsigned int lockFlag = 0;

//flag used to indicate demanded context switch
volatile int demanded_context_switch = 0;

#define lockMacro lockFlag=1;

#define unlockMacro lockFlag=0;\
if (demanded_context_switch) {\
dispatch();\
}

//Process Control Block used to save the context of the running thread
//and to initialize a new context
//used during context switching
class PCB{
public:
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned finished;
	int quantum;
};

PCB *p[3];
volatile PCB* running;



volatile int nextThread = 2;
PCB* getNextPCBToExecute(){
	if (nextThread == 1)
		nextThread = 2;
	else
		nextThread = 1;
	if (p[nextThread]->finished) {
		if (nextThread == 1)
			nextThread = 2;
		else
			nextThread = 1;
		if (p[nextThread]->finished)
			nextThread = 0;
	}
	return p[nextThread];
}


//helper variables for the timer interrupt
unsigned tsp;
unsigned tss;
unsigned tbp;

//counter for context switching
volatile int counter = 20;

void interrupt timer(){	// interrupt routine
	if (!demanded_context_switch) counter--;
	if (counter == 0 || demanded_context_switch) {
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
			/*if (!running->finished)
				Scheduler::put((PCB *)running);
			running = Scheduler::get();*/
			running = getNextPCBToExecute();

			//restores stack and base pointer from the new PCB
			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

			//resets counter to the specified time quantum
			//for the new running thread
			counter = running->quantum;
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

//synchronous context switch
void dispatch(){
	asm cli;
	demanded_context_switch = 1;
	timer();
	asm sti;
}

//old interrupt routine address
unsigned oldTimerOFF, oldTimerSEG;

// sets a new timer interrupt routine
void inic(){
	asm{
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
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

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

void exitThread(){
	running->finished = 1;
	dispatch();
}  

void a(){
	for (int i =0; i < 30; ++i) {
		lockMacro;
		cout <<"u a() i = "<<i<< endl;
		unlockMacro;
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}

void b(){
	for (int i =0; i < 30; ++i) {
		lockMacro;
		cout<<"u b() i = "<<i<<endl;
		unlockMacro;
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}

//giving a start context to the thread
//and initializing all fields into the PCB
void createProcess(PCB *newPCB, void (*body)()){
	unsigned* st1 = new unsigned[1024];

	//sets the I flag in the PSW register
	//of the new thread
	st1[1023] =0x200;

#ifndef BCC_BLOCK_IGNORE
	//sets the starting PC of the thread
	st1[1022] = FP_SEG(body);
	st1[1021] = FP_OFF(body);
	//"pushing all registers on the stack"
	newPCB->sp = FP_OFF(st1+1012);
	newPCB->ss = FP_SEG(st1+1012);
	newPCB->bp = FP_OFF(st1+1012);
#endif
	newPCB->finished = 0;
}



void doSomething(){
	lockMacro;
	p[1] = new PCB();
	createProcess(p[1],a);
	cout << "napravio a" << endl;
	p[1]->quantum = 40;
	Scheduler::put(p[1]);

	p[2] = new PCB();
	createProcess(p[2],b);
	cout<<"napravio b"<<endl;
	p[2]->quantum = 20;
	Scheduler::put(p[2]);

	p[0] = new PCB();

	running = p[0];
	unlockMacro;

	for (int i = 0; i < 30; ++i) {
		lockMacro;
		cout << "main " << i << endl;
		unlockMacro;

	for (int j = 0; j< 30000; ++j)
		for (int k = 0; k < 30000; ++k);
	}
	cout << "Happy End" << endl;
}

int main(){
	
  inic();

  doSomething();
  
  restore();
 
  return 0;
}

