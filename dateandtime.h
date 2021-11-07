// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _dateandtime_H
#define	_dateandtime_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Initialise what day it is when device is first switched on (using manual input)
typedef struct {
    signed int year;
    signed char month,date,day,hour,minute,second,sunrise_hour,sunrise_minute,sunrise_second;
} dateandtime;

dateandtime time_incre(dateandtime current);
dateandtime daylightsavingstime_toggle(dateandtime current);
dateandtime date_check(dateandtime current);
dateandtime sunrise(dateandtime current);
dateandtime sun_sync(dateandtime current);

#endif	/* _dateandtime_H */