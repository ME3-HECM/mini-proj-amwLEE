// This is a guard condition so that contents of this file are not included more than once
#ifndef _LEDarray_H
#define	_LEDarray_H

#include <xc.h>

#define _XTAL_FREQ 64000000

// Function prototypes
void LEDarray_init(void);
void LEDarray_disp_bin(signed char number);

#endif	/* _LEDarray_H */