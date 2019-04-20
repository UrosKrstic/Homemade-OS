#include <iostream.h>
#include "thread.h"
#include "timer.h"
#include "PCB.h"

class AB : public Thread {
public:
	char c;
	AB(unsigned long l, unsigned int t, char c) : Thread(l,t) {
		this->c = c;
	}
	~AB() {waitToComplete();}
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


int userMain(int argc, char* argv[]) {
    AB* a = new AB(1024, 40, 'a');
	AB* b = new AB(1024, 40, 'b');
	lockMacro;
	GlobalPCBList->printList();
	unlockMacro;
	a->start();
	b->start();
	delete b;
	lockMacro;
	GlobalPCBList->printList();
	Thread* t = Thread::getThreadById(2);
	if (t == nullptr) {
		t = Thread::getThreadById(1);
		cout << "not found 2"<< endl;
	}
	if (t == a)
		cout << "Hurray" << endl;
	unlockMacro;
	delete a;
	for (int i = 0; i < 30; ++i) {
		lockMacro;
		cout << "main " << i << endl;
		unlockMacro;
		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}
	cout << "Happy End" << endl;

    return 0;
}