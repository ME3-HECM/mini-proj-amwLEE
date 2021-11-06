// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _comparator_H
#define	_comparator_H

#include <xc.h> // include processor files - each processor file is guarded.

#define _XTAL_FREQ 64000000

void DAC_init(void);
void Comp1_init(void);

#endif	/* _comparator_H */