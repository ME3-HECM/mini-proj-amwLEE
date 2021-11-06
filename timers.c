#include <xc.h>
#include "timers.h"

/*****************************
 * Function to set up timer 0
*****************************/
void Timer0_init(void) {
    T0CON1bits.T0CS=0b010;      // Fosc/4 (see datasheet pg 354)
    T0CON1bits.T0ASYNC=1;       // Needed to ensure correct operation when Fosc/4 used as clock source (see datasheet errata)
    T0CON1bits.T0CKPS=0b1000;   // Adjust prescaler to 1:256 (see datasheet pg 369, see below for explanations)
    T0CON0bits.T016BIT=1;       // 16 bit mode (see datasheet pg 358)
	// Calculate required prescaler using T_int=1/65535=4*PS/64000000.
    // Thus, we need a prescaler 1:244.1443503... to get an overflow exactly every second.
    // However, such a prescaler does not exist, so the closest one is 1:256. This will cause an overflow every 1.04856 seconds.
    
    // It's a good idea to initialise the timer registers at a fine-tuned value
    // to correct our error of 0.04856 when the timer overflows.
    // We need to start the timer at a higher value to reduce the time period by 0.04856 seconds
    // before the next overflow occurs.
    // Thus, we start the timer at n=65535-(1/(4*256/64000000))=3035.
    // Now my device will be 100% accurate for as long as the device is working.
    // Write High reg first, update happens when low reg is written to
    TMR0H=0b1011;               // 16-bit Timer 0 most significant byte
    TMR0L=0b11011011;           // 16-bit Timer 0 least significant byte
    T0CON0bits.T0EN=1;          // Start the timer
}

/******************************************************************************************************
 * Function to return the full 16 bit timer value
 * Note TMR0L and TMR0H must be read in the correct order, or TMR0H will not contain the correct value
******************************************************************************************************/
unsigned int get16bitTMR0val(void) {
    // Add your code to get the full 16 bit timer value here
    unsigned int tmpval; // Variable to store full 16 bit timer value
    
    // Note it is unreliable to add both TMR0L and TMR0H in a single line of code,
    // as the compiler is free to rearrange operations between sequence points.
    // To guarantee a correct read, TMR0L must be read first on its own line,
    // before reading/adding TMR0H.
    tmpval = TMR0L;                 // Read TMR0L
    tmpval = tmpval | (TMR0H<<8);   // Read TMR0H and use inclusive OR to combine TMR0L and TMR0H to give full 16 bit timer value
    
    return tmpval; // Return the full 16 bit timer value
}