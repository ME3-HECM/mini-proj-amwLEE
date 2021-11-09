#include <xc.h>
#include "main.h"

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
    current.month = 10;             // Month (January==1, ..., December==12)
    current.date = 30;              // Date (1, ..., 31)
    current.day = 6;                // Day of the week (Monday==1, ..., Sunday==7)
    current.dst = 1;                // Are we currently in daylight savings time? (BST=1, GMT=0)
    current.hour = 18;              // Hour using 24-hour clock system (12am==0, ..., 9am==9, ..., 12pm==12, ..., 5pm==17, ...)
    current.minute = 0;             // Minute (0, ..., 59)
    current.second = 0;             // Second (0, ..., 59)
    current.sunrise_hour = 7;       // The hour that sunrise occurred today (4am==4, ..., 8am==8)
    current.sunrise_minute = 0;     // The minute that sunrise occurred today (0, ..., 59)
    current.sunrise_second = 0;     // The second that sunrise occurred today (0, ..., 59)
          
    // Call your initialisation functions to set up the hardware modules
    ADC_init();                     // Function to initialise ADC module and set it up to sample on pin RF7
    Comp1_init();                   // Function to set up comparator to compare RF7 to the DAC output voltage
    Timer0_init();                  // Function to set up Timer 0
    Interrupts_init(current);       // Function to turn on interrupts and set if priority is used
    LED1_init(current);             // Function to initialise pins to drive LED1 (the street light)
    LED2_init();                    // Function to initialise pins to drive LED2 (the minute indicator - flashes every minute)
    LEDarray_init();                // Function to initialise pins to drive the LED array (the hour indicator - displays the hour in binary)
    LCD_init(current);              // Function to initialise the LCD after power on (the date and time indicator - displays the date and time digitally on the screen)
    
    // Infinite while loop (program runs indefinitely - or more realistically, until the hardware fails)
    while (1) {
        current = sunrise_sunset(current);  // Check if sunrise or sunset has just occurred (toggled by high priority interrupt)
                                            // If sunrise, record today's sunrise timing
                                            // If sunset, calculate what time solarnoon was recorded today and adjust clock to sync with sun
        
        current = time_incre(current);      // Check if the timer has just overflowed (toggled by low priority interrupt)
                                            // If yes, increase time by 1 second
                                            // If no, pass
        
        current = LED_toggle(current);      // Check if the current time is between 1am-5am
                                            // If yes, disable comparator interrupt, and turn off the street light
                                            // If no, enable comparator interrupt, and check if it's still dark enough for the street lights to be turned on
                                            // or whether it's bright enough to record sunrise
        
        LEDarray_disp_bin(current.hour);    // Display the current hour on the LEDarray in binary
    }
}