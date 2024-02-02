// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _timers_testmode_H
#define	_timers_testmode_H

#include <xc.h> // include processor files - each processor file is guarded.

#define _XTAL_FREQ 64000000

//function prototypes
void Timer0_init(void);
//unsigned int get16bitTMR0val(void);

#endif	/* _timers_H */