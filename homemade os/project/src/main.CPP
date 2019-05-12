//memory model in use: huge
//simple kernel implementation

#include <iostream.h>
#include <dos.h>
#include "thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "timer.h"

int userMain(int argc, char* argv[]);
void hand1();

int main(int argc, char* argv[]) {
	
	inic();
	((PCB*)PCB::running)->registerHandler(1, hand1);
	int return_value = userMain(argc, argv);
	restore();
  return return_value;
}
