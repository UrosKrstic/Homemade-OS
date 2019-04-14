//memory model in use: huge
//simple kernel implementation

#include <iostream.h>
#include <dos.h>
#include "thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "timer.h"

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

void doSomething(){
	lockMacro;
	p[1] = new PCB(1024, 40, a);
	cout << "napravio a" << endl;
	Scheduler::put(p[1]);

	p[2] = new PCB(1024, 40, b);
	cout<<"napravio b"<<endl;
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
