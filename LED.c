#include <xc.h>
#include "LED.h"
#include "ADC.h"
#include "dateandtime.h"

/**************************************************************
 * LED1_init
 * Function to initialise pins to drive LED1 (the street light)
 **************************************************************/
void LED1_init(dateandtime current) {
    // Check if initialised time and surrorunding brightness warrants the street light to be switched on immediately
    // Calculate threshold value for "surrounding brightness" (value should correspond to that used in the comparator/interrupt)
    // 10-bit ADC value
    // Min is 0V = 0
    // Max is 3.3V = 2^10-1 = 1023
    // Adjust threshold corresponding to that used in comparator/interrupt 2.87V ~ 890=0b1101111010
    // Take only the 8msb 0b11011110
    if ((current.hour<1||current.hour>=5) && (ADC_getval()>=0b11011110)) {  // Check if initialised time is outside 1am-5am and surroundings is already dark
        LATDbits.LATD7 = 1;                                                 // If yes, set initial output state for LED2 as on (turn street light on)
    } else {LATDbits.LATD7 = 0;}                                            // If no, set initial output state for LED2 as off (turn street light off)
    
    TRISDbits.TRISD7 = 0;                                                   // Set TRIS value for RD7 pin as output
}

/*****************************************************************************************
 * LED2_init
 * Function to initialise pins to drive LED2 (the minute indicator - flashes every minute)
 *****************************************************************************************/
void LED2_init(void) {
    LATHbits.LATH3 = 1;             // Set initial output state for LED2 as on
    TRISHbits.TRISH3 = 0;           // Set TRIS value for RH3 pin as output
}

/***************************************************************************************************************************************************************
 * LED_toggle
 * Function to check if the current time is between 1am-5am
 * If yes, disable comparator interrupt, and turn off the street light
 * If no, enable comparator interrupt, and check if it's still dark enough for the street lights to be turned on or whether it's bright enough to record sunrise
 ***************************************************************************************************************************************************************/
dateandtime LED_toggle (dateandtime current) {
    if (current.hour==1 && current.minute==0 && current.second==0) {        // If current time is 1am
        PIE2bits.C1IE = 0;                                                  // Disable comparator interrupt (so street light won't be turned on by accident)
        LATDbits.LATD7 = 0;                                                 // and turn off street light immediately
    } else if (current.hour==5 && current.minute==0 && current.second==0) { // If current time is 5am
        PIE2bits.C1IE = 1;                                                  // Enable comparator interrupt again (so street light can be turned on and off as needed)
        if (ADC_getval()>=0b11011110) {LATDbits.LATD7 = 1;}                 // and turn on the street light if it's still dark
        else {current=sunrise(current);}                                    // or record 5am as sunrise timing if it's already bright
    }
    
    return current;                                                         // Update current timing (which may include today's new sunrise timing)
}