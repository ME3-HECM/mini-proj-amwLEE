// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _timers_H
#define	_timers_H

#include <xc.h> // include processor files - each processor file is guarded.

#define _XTAL_FREQ 64000000

//function prototypes
void LEDarray_init(void);
void LEDarray_disp_bin(unsigned int number);

#endif	/* _timers_H */