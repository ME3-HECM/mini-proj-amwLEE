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
#include <xc.h>             // XC8 compiler standard library
#include "dateandtime.h"    // Time header file in current directory
#include "ADC.h"            // ADC header file in current directory
#include "comparator.h"     // Comparator header file in current directory
#include "interrupts.h"     // Interrupts header file in current directory
#include "LCD.h"            // LCD header file in current directory
#include "LED.h"            // LED header file in current directory
#include "LEDarray.h"       // LED array header file in current directory
#include "timers.h"         // Timers header file in current directory


/**********************************************************
 * A definition calls for a macro substitution
 * See The C programming language, second edition, pp.89-91
 **********************************************************/
#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz


/***************
 * Main function
 ***************/
void main(void) {
    // Initialise today's date and time
    struct dateandtime init;
    init.year = 2021;
    init.month = 11;
    init.date = 7;
    init.day = 7;
    init.hour = 17;
    init.minute = 0;
    init.second = 0;
    init.sunrise_hour = 7;
    init.sunrise_minute = 0;
    init.sunrise_second = 0;
    
    // Call your initialisation functions to set up the hardware modules
    ADC_init();         // Function used to initialise ADC module and set it up to sample on pin RA3
    Interrupts_init();  // Function to turn on interrupts and set if priority is used
    Comp1_init();       // Function to set up comparator to compare RF7 to the DAC output voltage
    LED1_init();        // Function to initialise pins to drive LED1
    LED2_init();        // Function to initialise pins to drive LED2
    LEDarray_init();    // Function to initialise pins to drive the LED array
    Timer0_init();      // Function to set up timer 0
    
    struct dateandtime current;
    current = init;
    while (1) {
        LEDarray_disp_bin(current.hour);
        LED_toggle(current);
    }
}