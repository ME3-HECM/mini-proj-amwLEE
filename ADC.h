// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _ADC_H
#define	_ADC_H

#include <xc.h> // include processor files - each processor file is guarded.

#define _XTAL_FREQ 64000000

void ADC_init(void);
unsigned int ADC_getval(void);

#endif	/* _ADC_H */