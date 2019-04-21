#include <iostream.h>
#include "thread.h"
#include "semaphor.h"
#include "timer.h"
#include "PCB.h"

Semaphore *sem;
Semaphore *sem2;

class A : public Thread {
public:
	char c;
	A(unsigned long l, unsigned int t, char c) : Thread(l,t) {
		this->c = c;
	}
	~A() {waitToComplete();}
protected:
	void run();
};

void A::run() {
	for (int i = 0; i < 5; ++i) {
		sem2->wait(2);
		lockMacro;
		cout<<"u " << c << ":" << "() i = "<< i <<endl;
		unlockMacro;
	for (int k = 0; k<20000; ++k)
		for (int j = 0; j <30000; ++j);
	}
}

class B : public Thread {
public:
	char c;
	B(unsigned long l, unsigned int t, char c) : Thread(l,t) {
		this->c = c;
	}
	~B() {waitToComplete();}
protected:
	void run();
};

void B::run() {
	for (int i = 0; i < 30; ++i) {
		lockMacro;
		cout<<"u " << c << ":" << "() i = "<< i <<endl;
		unlockMacro;
		//sem->signal(3);
	for (int k = 0; k<30000; ++k)
		for (int j = 0; j <30000; ++j);
	}
}

class C : public Thread {
public:
	char c;
	C(unsigned long l, unsigned int t, char c) : Thread(l,t) {
		this->c = c;
	}
	~C() {waitToComplete();}
protected:
	void run();
};

void C::run() {
	for (int i = 0; i < 5; ++i) {
		int p = sem->wait(1);
		lockMacro;
		cout<<"u " << c << ":" << "() i = "<< i << " " << p <<endl;
		unlockMacro;
	for (int k = 0; k<20000; ++k)
		for (int j = 0; j <30000; ++j);
	}
}


int userMain(int argc, char* argv[]) {
    A* a = new A(1024, 40, 'a');
	C* c = new C(1024, 40, 'c');
	A* d = new A(1024, 40, 'd');
	B* b = new B(1024, 40, 'b');
	sem = new Semaphore(0);
	sem2 = new Semaphore(3);
	lockMacro;
	GlobalPCBList->printList();
	unlockMacro;

	a->start();
	b->start();
	c->start();
	d->start();
	
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

	for (int i = 0; i < 30; ++i) {
		lockMacro;
		cout << "main " << i << endl;
		unlockMacro;
		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}
	cout << "Happy End" << endl;

	delete a;
	delete b;
	delete c;
	delete d;
	cout << "Happy End" << endl;
	delete sem;
	delete sem2;
	cout << "Happy End" << endl;
    return 0;
}