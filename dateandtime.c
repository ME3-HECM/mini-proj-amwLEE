#include <xc.h>
#include <stdio.h>
#include "dateandtime.h"
#include "interrupts.h"
#include "LCD.h"

/************************************************************************************
 * daylightsavingstime_toggle
 * Function to check whether the clocks should change for daylight savings time today
 * On the last Sunday of March every year, the clocks go forward by 1 hour at 1am
 * On the last Sunday of October every year, the clocks go backwards by 1 hour at 2am
 ************************************************************************************/
dateandtime daylightsavingstime_toggle(dateandtime current) { // Take current date and time as input
    
    if (current.month==3 && (current.date+7)>31 && current.day==7 && current.hour==1) {         // If it is March, we're in the last week, it's a Sunday, and it's 1am
        current.hour = current.hour+1;                                                          // Move the clock forward by 1 hour
    } else if (current.month==10 && (current.date+7)>31 && current.day==7 && current.hour==2) { // If it is October, we're in the last week, it's a Sunday, and it's 2am
        current.hour = current.hour-1;                                                          // Move the clock backwards by 1 hour
    }
    
    return current; // Update the new time
}

/********************************************************************************
 * date_check
 * Function to check whether a new month should begin today
 * If yes, reset the "date" to 1, and increase the "month" by 1
 * E.g. This ensures "1 December 2021" is displayed instead of "32 November 2021"
 ********************************************************************************/
dateandtime date_check(dateandtime current) { // Take current date and time as input
    
    // If it is a leap year, check if February 29 just passed
    // If it is not a leap year, check if February 28 just passed
    // Also check if January 31, March 31, May 31, July 31, August 31, October 31 or December 31 just passed
    // Also check if April 30, June 30, September 30 or November 30 just passed
    if ((current.year%4==0 && current.month==2 && current.date>29)\
            || (current.year%4!=0 && current.month==2 && current.date>28)\
            || ((current.month==1 || current.month==3 || current.month==5 || current.month==7 || current.month==8 || current.month==10 || current.month==12) && current.date>31)\
            || ((current.month==4 || current.month==6 || current.month==9 || current.month==11) && current.date>30)) {
        current.month = current.month+1;    // Begin a new month
        current.date = 1;                   // Reset the "date" to the 1st
        
        if (current.month>12) {             // Special case if December 31 just passed
            current.year = current.year+1;  // Begin a new year
            current.month = 1;              // Reset the "month" to January
        }
    }
    
    return current; // Update the current date
}

/**********************************************************************************************
 * time_incre
 * Function to increment time every second that passes by
 * This function is called every time the timer overflows (indicating 1 second has just passed)
 **********************************************************************************************/
dateandtime time_incre(dateandtime current){ // Take current date and time as input
    
    if (time_flag==1) {                     // If the timer has just overflowed
        current.second = current.second+1;  // Increment the second by 1
        
        char buf1[40];                      // Declare a variable buffer to hold the position of 
        LCD_setline(2);
        sprintf(buf1,"%02d:%02d:%02d",current.hour,current.minute,current.second);
        LCD_sendstring(buf1);
        
        if (current.second>59) {
            LATHbits.LATH3 = !LATHbits.LATH3;   // Toggle the LED on RH3 when the LDR goes from light to dark
            current.second = 0;
            current.minute = current.minute+1;
            if (current.minute>59) {
                current.minute = 0;
                current.hour = current.hour+1;
                current = daylightsavingstime_toggle(current);
                if (current.hour>23) {
                    current.hour = 0;
                    current.date = current.date+1;
                    current = date_check(current);
                    
                    char buf2[40];
                    LCD_setline(1);
                    sprintf(buf2,"%04d-%02d-%02d",current.year,current.month,current.date);
                    LCD_sendstring(buf2);
                    
                    current.day = current.day+1;
                    if (current.day>7) {
                        current.day = 1;
                    }
                }
            }
        }
        time_flag=0; // Reset the timer flag
    }
    
    return current;
}

/*****************************************************
 * sunrise
 * Function to record what time sunrise occurred today
 *****************************************************/
dateandtime sunrise(dateandtime current) {      // Take current date and time as input
    
    current.sunrise_hour = current.hour;        // Take the current hour as the sunrise hour
    current.sunrise_minute = current.minute;    // Take the current minute as the sunrise minute
    current.sunrise_second = current.second;    // Take the current second as the sunrise second
    
    return current;                             // Update the new sunrise timing
}

/****************************************************************************************************
 * sun_sync
 * Function to check synchronicity with sun and adjust accordingly
 * This is done every day when sunset occurs
 * Calculate what time "solarnoon" was recorded today (the midpoint between sunrise and sunset today)
 * Adjust the clock such that solarnoon would have occured at exactly 12pm
 ****************************************************************************************************/
dateandtime sun_sync(dateandtime current) { // Take the current date and time as input
    
    signed char solarnoon_hour = (current.sunrise_hour + current.hour)/2;       // Calculate what time the solarnoon hour was recorded today
    signed char solarnoon_minute = (current.sunrise_minute + current.minute)/2; // Calculate what time the solarnoon minute was recorded today
    signed char solarnoon_second = (current.sunrise_second + current.second)/2; // Calculate what time the solarnoon second was recorded today
    
    current.second = current.second - (solarnoon_second-0); // Adjust the current second such that solarnoon today would have happened at exactly the 0 second
    if (current.second<0) {                                 // The above adjustment might lead to an undesirable "negative second" (hence why "current" has been configured as "signed char/int")
        current.second = current.second+60;                 // Correct the negative second
        current.minute = current.minute-1;                  // Correct the corresponding minute
    }
    current.minute = current.minute - (solarnoon_minute-0); // Adjust the current minute such that solarnoon today would have happened at exactly the 0 minute
    if (current.minute<0) {                                 // The above adjustment might lead to an undesirable "negative minute" (hence why "current" has been configured as "signed char/int")
        current.minute = current.minute+60;                 // Correct the negative minute
        current.hour = current.hour-1;                      // Correct the corresponding hour
    }
    current.hour = current.hour - (solarnoon_hour-12);      // Adjust the current hour such that solarnoon today would have happened at exactly the 0 hour
                                                            // Note that the hour should not have the same issues of "negative hour" as above
                                                            // This is because we make this adjustment every time sunset occurs
                                                            // Thus the "current hour" would definitely be well passed solarnoon
                                                            // Any adjustments would still deem the hour positive and correct
    
    return current; // Update the adjusted time
}

/*********************************************
 * sunrise_sunset
 * Function to check if it is sunrise or sunset
 *********************************************/
dateandtime sunrise_sunset(dateandtime current) { // Take the current date and time as input
    
    if (sunrise_flag==1) {          // If sunrise has just occurred,
    current=sunrise(current);       // update today's sunrise timing in "current"
    sunrise_flag=0;                 // and reset the sunrise flag
    } else if (sunset_flag==1) {    // If sunset has just occurred,
        current=sun_sync(current);  // check what time solarnoon was recorded today and sync the clock with the sun
        sunset_flag=0;              // and reset the sunset flag
    }
    
    return current; // Update the new time records
}