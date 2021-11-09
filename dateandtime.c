#include <xc.h>
#include <stdio.h>
#include "dateandtime.h"
#include "interrupts.h"
#include "LCD.h"

/********************
 * Check for daylight savings time
 ******************/
dateandtime daylightsavingstime_toggle(dateandtime current) {
    
    if (current.month==3 && (current.date+7)>31 && current.day==7 && current.hour==1) {
        current.hour = current.hour+1;
    } else if (current.month==10 && (current.date+7)>31 && current.day==7 && current.hour==2) {
        current.hour = current.hour-1;
    }
    
    return current;
}


/******************************
 * Check for date
 *******************************/
dateandtime date_check(dateandtime current) {
    
    if ((current.year%4==0 && current.month==2 && current.date>29)\
            || (current.year%4!=0 && current.month==2 && current.date>28)\
            || ((current.month==1 || current.month==3 || current.month==5 || current.month==7 || current.month==8 || current.month==10 || current.month==12) && current.date>31)\
            || ((current.month==4 || current.month==6 || current.month==9 || current.month==11) && current.date>30)) {
        current.month = current.month+1;
        current.date = 1;
        
        if (current.month>12) {
            current.year = current.year+1;
            current.month = 1;
        }
    }
    
    return current;
}


/******************************
 * Check for time
 *******************************/
dateandtime time_incre(dateandtime current){
    if (time_flag==1) { // If the timer has just overflowed
        current.second = current.second+1;
        
        char buf1[40];
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


/***********************
 * Check sunrise time
 ************************/
dateandtime sunrise(dateandtime current) {
    current.sunrise_hour = current.hour;
    current.sunrise_minute = current.minute;
    current.sunrise_second = current.second;
    
    return current;
}


/**********************
 * Check synchronicity with sun
 *********************/
dateandtime sun_sync(dateandtime current) {
    
    signed char solarnoon_hour = (current.sunrise_hour + current.hour)/2;
    signed char solarnoon_minute = (current.sunrise_minute + current.minute)/2;
    signed char solarnoon_second = (current.sunrise_second + current.second)/2;
    
    current.second = current.second - (solarnoon_second-0);
    if (current.second<0) {
        current.second = current.second+60;
        current.minute = current.minute-1;
    }
    current.minute = current.minute - (solarnoon_minute-0);
    if (current.minute<0) {
        current.minute = current.minute+60;
        current.hour = current.hour-1;
    }
    current.hour = current.hour - (solarnoon_hour-12);
    
    return current;
}

/*******************
 * Function to check for sunrise and sunset
 *****************/
dateandtime sunrise_sunset(dateandtime current) {
    if (sunrise_flag==1) {                  // If sunrise has just occurred,
    current=sunrise(current);           // update today's sunrise timing in the structure "current"
    sunrise_flag=0;                     // and reset the sunrise flag
    } else if (sunset_flag==1) {            // If sunset has just occurred,
        current=sun_sync(current);          // check what time solarnoon was recorded today, and sync the clock with the sun
        sunset_flag=0;                      // and reset the sunset flag
    }
    return current;
}