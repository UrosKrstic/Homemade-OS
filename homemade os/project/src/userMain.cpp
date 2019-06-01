// #include <iostream.h>
// #include "event.h"
// #include "thread.h"
// #include "semaphor.h"
// #include "timer.h"
// #include "PCB.h"
// #include <stdio.h>
// #include <stdarg.h>
// #include <stdlib.h>

// int syncPrintf(const char *format, ...)
// {
// 	int res;
// 	va_list args;
// 	lockMacro;
// 	va_start(args, format);
// 	res = vprintf(format, args);
// 	va_end(args);
// 	unlockMacro;
// 	return res;
// }

// Semaphore sem1(0), sem2(0);

// void hand1() { syncPrintf("hand1\n"); }
// void hand2() { syncPrintf("hand2\n"); }
// void hand3() { syncPrintf("hand3\n"); }
// void hand4() { syncPrintf("hand4\n"); }

// class Nit : public Thread {
// public:
// 	Nit(): Thread() {};
// 	~Nit() {
// 		waitToComplete();
// 	}
// protected:
// 	void run();
// };

// void Nit::run() {
// 	syncPrintf("Pocela nit n\n");
// 	registerHandler(3, hand2);
// 	registerHandler(6, hand1);
// 	syncPrintf("Spremio svoje dodatne handlere Nit n\n");

// 	sem1.signal();
// 	sem2.wait(0);

// 	sem1.signal();
// 	sem2.wait(0);

// 	sem1.signal();
// 	sem2.wait(0);

// 	sem1.signal();
// 	sem2.wait(0);
// }

// class TestThread : public Thread {
// public:
// 	TestThread(): Thread() {};
// 	~TestThread() {
// 		waitToComplete();
// 	}
// protected:
// 	void run();
// };

// void TestThread::run() {
// 	syncPrintf("Pocela nit t\n");
// 	Nit n;
// 	n.start();

// 	sem1.wait(0);
// 	syncPrintf("Zove odgovarajuce signale niti n\n");
// 	for (int i = 1; i < NUM_OF_SIGNALS; i++)
// 		n.signal(i);
	
// 	sem2.signal();
// 	sem1.wait(0);

// 	n.blockSignal(3);
// 	n.blockSignalGlobally(6);
// 	for (i = 1; i < NUM_OF_SIGNALS; i++)
// 		n.signal(i);

// 	sem2.signal();
// 	sem1.wait(0);

// 	n.unblockSignal(3);
// 	n.unblockSignalGlobally(6);

// 	for (i = 0; i < NUM_OF_SIGNALS; i++) {
// 		n.unregisterAllHandlers(i);
// 	}
// 	for (i = 1; i < NUM_OF_SIGNALS; i++)
// 		n.signal(i);


// 	sem2.signal();
// 	sem1.wait(0);

// 	n.signal(0); // kill signal
// 	sem2.signal(); 
// 	n.waitToComplete();
// }

// int userMain(int argc, char* argv[]) {

// 	// SignalHandlerList shl;
// 	// shl.insertHandler(hand1);
// 	// shl.insertHandler(hand2);
// 	// shl.insertHandler(hand3);
// 	// shl.insertHandler(hand4);
// 	// for (SignalHandler sh = shl.begin(); sh != shl.end(); sh = shl.getNext()) {
// 	// 	(*sh)();
// 	// }
// 	// shl.swap(hand4, hand1);
// 	// shl.swap(hand2, hand3);
// 	// for (sh = shl.begin(); sh != shl.end(); sh = shl.getNext()) {
// 	// 	(*sh)();
// 	// }
// 	// shl.clearList();
// 	// for (sh = shl.begin(); sh != shl.end(); sh = shl.getNext()) {
// 	// 	(*sh)();
// 	// }

// 	TestThread t;
// 	t.start();

// 	t.registerHandler(3, hand1);
// 	t.registerHandler(11, hand3);
// 	t.registerHandler(15, hand4);
// 	t.registerHandler(1, hand1);

// 	t.signal(3);
// 	t.signal(11);
// 	t.signal(15);
// 	syncPrintf("all signals prepared for TestThread\n");
// 	t.waitToComplete();
// 	return 0;
// }