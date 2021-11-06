/***************************************************
 * CONFIG1L (configuration word 1) - oscillators
 * See PIC18(L)F67K40 Data Sheet 40001841D, pp.28-29
 ***************************************************/
#pragma config FEXTOSC = HS         // FEXTOSC external oscillator mode selection bits (HS (crystal oscillator) above 8 MHz; PFM (program memory) set to high power)
#pragma config RSTOSC = EXTOSC_4PLL // Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)


/***********************************************************
 * CONFIG3L (configuration word 3) - windowed watchdog timer
 * See PIC18(L)F67K40 Data Sheet 40001841D, pp.32-33
 ***********************************************************/
#pragma config WDTE = OFF   // WDT operating mode bits (WDT enabled regardless of sleep)


/**************************************************************************************
 * File inclusion makes it easy to handle collections of #defines and declarations etc.
 * See The C programming language, second edition, pp.88-89
 **************************************************************************************/
#include <xc.h>         // XC8 compiler standard library
#include "ADC.h"        // ADC header file in current directory
#include "comparator.h" // Comparator header file in current directory
#include "interrupts.h" // Interrupts header file in current directory
#include "LCD.h"        // LCD header file in current directory
#include "LEDarray.h"   // LED array header file in current directory
#include "timers.h"     // Timers header file in current directory


/**********************************************************
 * A definition calls for a macro substitution
 * See The C programming language, second edition, pp.89-91
 **********************************************************/
#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz  


/***************
 * Main function
 ***************/
void main(void) {
    // Set up pin for output (connected to LED)
    LATHbits.LATH3 = 0;             // Set initial output state for LED on RH3 (low/off)
    TRISHbits.TRISH3 = 0;           // Set TRIS value for RH3 pin (output)
    
    // Call your initialisation functions to set up the hardware modules
    ADC_init();         // Function used to initialise ADC module and set it up to sample on pin RA3
    Comp1_init();       // Function to set up comparator to compare RF7 to the DAC output voltage
    Interrupts_init();  // Function to turn on interrupts and set if priority is used
    LEDarray_init();    // Function to initialise pins to drive the LED array
    Timer0_init();      // Function to set up timer 0
    
    while (1) {
        
    }
}