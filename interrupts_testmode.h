// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _interrupts_testmode_H
#define	_interrupts_testmode_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "dateandtime.h"

#define _XTAL_FREQ 64000000

void Interrupts_init(void);
void __interrupt(high_priority) HighISR();
void __interrupt(low_priority) LowISR();

#endif	/* _interrupts_H */