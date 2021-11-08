// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _LED_H
#define	_LED_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "dateandtime.h"

#define _XTAL_FREQ 64000000

//function prototypes
void LED1_init(dateandtime current);
void LED2_init(void);
dateandtime LED_toggle (dateandtime current);

#endif	/* _LED_H */