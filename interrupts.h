// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _interrupts_H
#define	_interrupts_H

#include <xc.h> // include processor files - each processor file is guarded.

#define _XTAL_FREQ 64000000

void Interrupts_init(void);
void __interrupt(high_priority) HighISR();

#endif	/* _interrupts_H */