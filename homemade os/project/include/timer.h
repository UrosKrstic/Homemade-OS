#ifndef _TIMER_H_
#define _TIMER_H_
#include <dos.h>

void interrupt timer();
void init();
void restore();

#endif #_TIMER_H_