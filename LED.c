#include <xc.h>
#include "LED.h"
#include "dateandtime.h"

void LED1_init(void) {
    // Set up pin for output (connected to LED)
    LATDbits.LATD7 = 0;             // Set initial output state for LED on RH3 (low/off)
    TRISDbits.TRISD7 = 0;           // Set TRIS value for RH3 pin (output)
}

void LED2_init(void) {
    // Set up pin for output (connected to LED)
    LATHbits.LATH3 = 0;             // Set initial output state for LED on RH3 (low/off)
    TRISHbits.TRISH3 = 0;           // Set TRIS value for RH3 pin (output)
}

void LED_toggle (dateandtime current) {
    if (current.hour==1) {
        PIE2bits.C1IE = 0;
        LATDbits.LATD7 = 0;
    } else if (current.hour==5) {
        PIE2bits.C1IE = 1;
    }
    
}