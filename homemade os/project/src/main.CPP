//memory model in use: huge
//simple kernel implementation

#include <iostream.h>
#include <dos.h>
#include "thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "timer.h"

class AB : public Thread {
public:
	char c;
	AB(unsigned long l, unsigned int t, char c) : Thread(l,t) {
		this->c = c;
	}
protected:
	void run();
};

void AB::run() {
		for (int i = 0; i < 30; ++i) {
			lockMacro;
			cout<<"u " << c << ":"
			
			 << "() i = "<< i <<endl;
			unlockMacro;
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
		}
	}


void doSomething() {
	AB* a = new AB(1024, 40, 'a');
	AB* b = new AB(1024, 40, 'b');
	a->start(); a->start();
	b->start();
	
	for (int i = 0; i < 30; ++i) {
		lockMacro;
		cout << "main " << i << endl;
		unlockMacro;
		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}
	cout << "Happy End" << endl;
}

int main() {
	
	PCB::init_running();

	inic();
	//korisnicki main
	//userMain(argc, argv);
	doSomething();
  
	restore();
 
  return 0;
}
