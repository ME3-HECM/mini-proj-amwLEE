#include <xc.h>
#include "LED.h"
#include "ADC.h"
#include "dateandtime.h"

// 8-bit ADC value to set output voltage
// Min is 0V = 0
// Max is 3.3V = 2^10-1 = 1023
// Adjust according to ambient light 2.87V ~ 890=0b1101111010
// The 8msb is therefore 0b11011110

void LED1_init(dateandtime current) {
    // Check if initialised timing and surrorunding brightness warrants the street light to be switched on immediately
    if ((ADC_getval()>=222) && (current.hour<1||current.hour>=5)) {
        LATDbits.LATD7 = 1; // If yes, set the initial LED state as on
    } else {
        LATDbits.LATD7 = 0; // If no, set the initial LED state as off
    }
    
    TRISDbits.TRISD7 = 0; // Set TRIS value for RD7 pin (output)
}

void LED2_init(void) {
    // Set up pin for output (connected to LED)
    LATHbits.LATH3 = 1;             // Set initial output state for LED on RH3 (high/on)
    TRISHbits.TRISH3 = 0;           // Set TRIS value for RH3 pin (output)
}

dateandtime LED_toggle (dateandtime current) {
    if (current.hour==1 && current.minute==0 && current.second==0) {
        PIE2bits.C1IE = 0;
        LATDbits.LATD7 = 0;
    } else if (current.hour==5 && current.minute==0 && current.second==0) {
        PIE2bits.C1IE = 1;
        if (ADC_getval()>=222) {LATDbits.LATD7 = 1;}
        else {current=sunrise(current);}           // update today's sunrise timing in the structure "current"
    }
    
    return current;
}