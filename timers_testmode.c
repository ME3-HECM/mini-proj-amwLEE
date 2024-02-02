#include <xc.h>
#include "timers_testmode.h"

/*****************************
 * Function to set up timer 0
*****************************/

void Timer0_init(void) {
    T0CON1bits.T0CS=0b010;      // Fosc/4 (see datasheet pg 354)
    T0CON1bits.T0ASYNC=1;       // Needed to ensure correct operation when Fosc/4 used as clock source (see datasheet errata)
    T0CON1bits.T0CKPS=0b1000;   // Adjust prescaler to 1:256 (see datasheet pg 369, see below for explanations)
    T0CON0bits.T016BIT=1;       // 16 bit mode (see datasheet pg 358)
	// Calculate required prescaler using T_int=(1/3600)/65535=4*PS/64000000.
    // Thus, we need a prescaler 1:0.0678178751... to get an overflow exactly every 0.000277777 ... second.
    // However, such a prescaler does not exist, so the closest one is 1:1. This will cause an overflow every 0.004095937... seconds.
    
    // It's a good idea to initialise the timer registers at a fine-tuned value
    // to correct our error of 0.003818159... seconds when the timer overflows.
    // We need to start the timer at a higher value to reduce the time period by 0.04856 seconds
    // before the next overflow occurs.
    // Thus, we start the timer at n=65535-((1/3600)/(4*1/64000000))=61090.55556 ~ 61091.
    // Now my device will be 100% accurate for as long as the device is working.
    // Write High reg first, update happens when low reg is written to
    TMR0H=0b11101110;           // 16-bit Timer 0 most significant byte
    TMR0L=0b10100011;           // 16-bit Timer 0 least significant byte
    T0CON0bits.T0EN=1;          // Start the timer
}