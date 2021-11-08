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
#pragma config WDTE = OFF           // WDT operating mode bits (WDT enabled regardless of sleep)


/**********************************************************
 * A definition calls for a macro substitution
 * See The C programming language, second edition, pp.89-91
 **********************************************************/
#define _XTAL_FREQ 64000000         // Note intrinsic _delay function is 62.5ns at 64,000,000Hz
#define _TEST_MODE ON               // Use a #define directive to switch between "normal" and "testing" modes for the code


/**************************************************************************************
 * File inclusion makes it easy to handle collections of #defines and declarations etc.
 * See The C programming language, second edition, pp.88-89
 **************************************************************************************/
#include <xc.h>                     // XC8 compiler standard library
#include "dateandtime.h"            // Time header file in current directory
#include "ADC.h"                    // ADC header file in current directory
#include "comparator.h"             // Comparator header file in current directory
#include "timers.h"                 // Timers header file in current directory
#include "interrupts.h"             // Interrupts header file in current directory
#include "LED.h"                    // LED header file in current directory
#include "LEDarray.h"               // LED array header file in current directory
#include "LCD.h"                    // LCD header file in current directory


/***************
 * Main function
 ***************/
void main(void) {
    // Initialise the date and time when the device is first switched on (in a dateandtime structure as defined in "dateandtime.h")
    // Information about today's sunrise timing can be found from a simple Google search,
    // however if unsure, you can leave the sunrise timing at its default time 07:00:00,
    // the code will be able to auto adjust back the timing after 2 days
    dateandtime current;
    current.year = 2021;            // Year (2021, 2022, ...)
    current.month = 11;             // Month (January==1, ..., December==12)
    current.date = 9;               // Date (1, ..., 31)
    current.day = 2;                // Day of the week (Monday==1, ..., Sunday==7)
    current.hour = 7;               // Hour using 24-hour clock system (12am==0, ..., 9am==9, ..., 12pm==12, ..., 5pm==17, ...)
    current.minute = 30;            // Minute (0, ..., 59)
    current.second = 0;             // Second (0, ..., 59)
    current.sunrise_hour = 7;       // The hour that sunrise occurred today (4am==4, ..., 8am==8)
    current.sunrise_minute = 0;     // The minute that sunrise occurred today (0, ..., 59)
    current.sunrise_second = 0;     // The second that sunrise occurred today (0, ..., 59)
    
    // Call your initialisation functions to set up the hardware modules
    ADC_init();                     // Function used to initialise ADC module and set it up to sample on pin RF7
    Comp1_init();                   // Function to set up comparator to compare RF7 to the DAC output voltage
    Timer0_init();                  // Function to set up timer 0
    Interrupts_init(current);       // Function to turn on interrupts and set if priority is used
    LED1_init(current);             // Function to initialise pins to drive LED1 (the street light)
    LED2_init();                    // Function to initialise pins to drive LED2 (the minute indicator - flashes every minute)
    LEDarray_init();                // Function to initialise pins to drive the LED array (the hour indicator - displays the hour in binary)
    LCD_init();                     // Function to initialise the LCD after power on (the date indicator - displays the date digitally on the screen)
    
    // Access global variables that are toggled by the interrupts
    extern volatile unsigned char sunrise_flag; // The high priority interrupt will toggle this variable when sunrise has occurred
    extern volatile unsigned char sunset_flag;  // The high priority interrupt will toggle this variable when sunset has occurred
    extern volatile unsigned char time_flag;    // The low priority interrupt will toggle this variable when a second has passed (when the timer has overflowed)
    
    // Infinite while loop (program runs indefinitely - or more realistically, until the hardware fails)
    while (1) {
        if (sunrise_flag==1) {                  // If sunrise has just occurred,
            current=sunrise(current);           // update today's sunrise timing in the structure "current"
            sunrise_flag=0;                     // and reset the sunrise flag
        } else if (sunset_flag==1) {
            current=sun_sync(current);          // If sunset has just occurred, check what time solarnoon was recorded today, and sync the clock with the sun
            sunset_flag=0;                      // and reset the sunset flag
        }
        
        if (time_flag==1) {                     // If the timer has just overflowed,
            current=time_incre(current);        // increment the current time by 1 second
            time_flag=0;                        // and reset the time flag
        }
        
        current = LED_toggle(current);          // Check if the current time is between 1am-5am
                                                // If yes, turn off the street light and disable interrupts
                                                // If no, check if it's still dark enough for the street lights to be turned on and enable interrupts
        
        LEDarray_disp_bin(current.hour);        // Display the current hour on the LEDarray in binary
    }
}