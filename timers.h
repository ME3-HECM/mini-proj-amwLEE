// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _timers_H
#define	_timers_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "main.h"

#define _XTAL_FREQ 64000000

#ifdef _TEST_MODE
#define TMR0H_bits 0b11101110
#define TMR0L_bits 0b10100011
#define prescaler 0b0000
#else
#define TMR0H_bits 0b1011
#define TMR0L_bits 0b11011011
#define prescaler 0b1000
#endif

//function prototypes
void Timer0_init(void);

#endif	/* _timers_H */