#include <iostream.h>
#include "thread.h"
#include "semaphor.h"
#include "timer.h"
#include "PCB.h"

// Semaphore *sem;
// Semaphore *sem2;

// class A : public Thread {
// public:
// 	char c;
// 	A(unsigned long l, unsigned int t, char c) : Thread(l,t) {
// 		this->c = c;
// 	}
// 	~A() {waitToComplete();}
// protected:
// 	void run();
// };

// void A::run() {
// 	for (int i = 0; i < 5; ++i) {
// 		sem2->wait(2);
// 		lockMacro;
// 		cout<<"u " << c << ":" << "() i = "<< i <<endl;
// 		unlockMacro;
// 	for (int k = 0; k<20000; ++k)
// 		for (int j = 0; j <30000; ++j);
// 	}
// }

// class B : public Thread {
// public:
// 	char c;
// 	B(unsigned long l, unsigned int t, char c) : Thread(l,t) {
// 		this->c = c;
// 	}
// 	~B() {waitToComplete();}
// protected:
// 	void run();
// };

// void B::run() {
// 	for (int i = 0; i < 30; ++i) {
// 		lockMacro;
// 		cout<<"u " << c << ":" << "() i = "<< i <<endl;
// 		unlockMacro;
// 		//sem->signal(3);
// 	for (int k = 0; k<30000; ++k)
// 		for (int j = 0; j <30000; ++j);
// 	}
// }

// class C : public Thread {
// public:
// 	char c;
// 	C(unsigned long l, unsigned int t, char c) : Thread(l,t) {
// 		this->c = c;
// 	}
// 	~C() {waitToComplete();}
// protected:
// 	void run();
// };

// void C::run() {
// 	for (int i = 0; i < 5; ++i) {
// 		int p = sem->wait(1);
// 		lockMacro;
// 		cout<<"u " << c << ":" << "() i = "<< i << " " << p <<endl;
// 		unlockMacro;
// 	for (int k = 0; k<20000; ++k)
// 		for (int j = 0; j <30000; ++j);
// 	}
// }


// int userMain(int argc, char* argv[]) {
//     A* a = new A(1024, 40, 'a');
// 	C* c = new C(1024, 40, 'c');
// 	A* d = new A(1024, 40, 'd');
// 	B* b = new B(1024, 40, 'b');
// 	sem = new Semaphore(0);
// 	sem2 = new Semaphore(3);
// 	lockMacro;
// 	GlobalPCBList->printList();
// 	unlockMacro;

// 	a->start();
// 	b->start();
// 	c->start();
// 	d->start();
	
// 	lockMacro;
// 	GlobalPCBList->printList();
// 	Thread* t = Thread::getThreadById(2);
// 	if (t == nullptr) {
// 		t = Thread::getThreadById(1);
// 		cout << "not found 2"<< endl;
// 	}
// 	if (t == a)
// 		cout << "Hurray" << endl;
// 	unlockMacro;

// 	for (int i = 0; i < 30; ++i) {
// 		lockMacro;
// 		cout << "main " << i << endl;
// 		unlockMacro;
// 		for (int j = 0; j< 30000; ++j)
// 			for (int k = 0; k < 30000; ++k);
// 	}
// 	cout << "Happy End" << endl;
	
// 	delete a;
// 	delete b;
// 	delete c;
// 	delete d;
// 	cout << "Happy End" << endl;
// 	delete sem;
// 	delete sem2;
// 	cout << "Happy End" << endl;
//     return 0;
// }

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	lockMacro;
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlockMacro;
	return res;
}

/*
 	 Test: Semafori sa spavanjem 3
*/

int t=-1;

Semaphore s(0);

class TestThread : public Thread
{
private:
	Time waitTime;

public:

	TestThread(Time WT): Thread(), waitTime(WT){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d waits for %d units of time.\n",getId(),waitTime);
	int r = s.wait(waitTime);
	s.signal();
	syncPrintf("Thread %d finished: r = %d\n", getId(),r);
}

void tick()
{
	/*
	t++;
	if(t)
		syncPrintf("%d\n",t);
		*/
}


#include <iostream.h>

Semaphore* mutex = 0;

class Znak : public Thread
{
public:
	Znak(char znak, int n) : Thread(), znak(znak), n(n) {}
	virtual ~Znak() { waitToComplete(); }

	void run()
	{
		for (long i = 0; i < 100000; i++)
		{
			if (mutex->wait(n)) {
				cout << znak;
				mutex->signal();
			}

		}

		if (mutex->wait(n)) {
			cout << endl << znak << " finished" << endl;
			mutex->signal();
		}
	}

private:
	char znak;
	int n;

};


int userMain(int argc, char* argv[]) {
	mutex = new Semaphore(1);

	Znak* a = new Znak('a', 10);
	Znak* b = new Znak('b', 15);
	Znak* c = new Znak('c', 20);

	a->start();
	b->start();
	c->start();

	delete a;
	delete b;
	delete c;

	if (mutex->wait(1)) {
		cout << endl << "userMain finished" << endl;
		mutex->signal();
	}

	delete mutex;



	return 0;
}

