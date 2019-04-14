#ifndef _PCB_H_
#define _PCB_H_

//Initializes I bit in PSW to 1
//all other bits to 0
#define PSW_INIT_VAL 0x200

//offsets on the stack 
#define PSW_OFFSET 1
#define RUN_METHOD_SEG_OFFSET 2
#define RUN_METHOD_OFF_OFFSET 3
#define ALL_REGISTERS_OFFSET 12

//Process Control Block used to save the context of the running thread
//and to initialize a new context
//used during context switching
class PCB{
public:
	unsigned long int stack_size;
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned finished;
	unsigned int time_slice;
	unsigned int unlimited_time_slice;
	PCB(unsigned long int stack_size = 4096, unsigned int time_slice = 2, void (*run_method)() = 0);
};

extern PCB *p[3];
extern volatile PCB* running;

#endif //_PCB_H_