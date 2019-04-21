#ifndef _TIMER_H_
#define _TIMER_H_

#include <dos.h>
#include "PCB.h"
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
extern volatile unsigned int lockFlag;

#define setLock lockFlag = 1;
#define clearLock lockFlag = 0;

//flag used to indicate demanded context switch
extern volatile int demanded_context_switch;

//elapsed time in the program
extern unsigned long time_in_ticks; // * 55ms

//disables context switching
#define lockMacro lockFlag=1;

//if a context switch was demanded
// it will after the end of the lock
#define unlockMacro lockFlag=0;\
if (demanded_context_switch) {\
dispatch();\
}

//interrupt routine
void interrupt tick();
// sets a new timer interrupt routine
void inic();
// restores old timer interrupt routine
void restore();
//synchronous context switch
void dispatch();

#endif //_TIMER_H_