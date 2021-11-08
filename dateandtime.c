#include <xc.h>
#include "dateandtime.h"

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
    
    current.second = current.second+1;
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
                current.day = current.day+1;
                if (current.day>7) {
                    current.day = 1;
                }
            }
        }
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