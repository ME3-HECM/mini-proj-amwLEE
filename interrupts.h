// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _interrupts_H
#define	_interrupts_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "dateandtime.h"
#include "main.h"

#define _XTAL_FREQ 64000000

#ifdef _TEST_MODE
#define TMR0H_bits 0b11101110
#define TMR0L_bits 0b10100011
#else
#define TMR0H_bits 0b1011
#define TMR0L_bits 0b11011011
#endif

void Interrupts_init(dateandtime current);
void __interrupt(high_priority) HighISR();
void __interrupt(low_priority) LowISR();

#endif	/* _interrupts_H */