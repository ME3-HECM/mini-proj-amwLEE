// This is a guard condition so that contents of this file are not included more than once
#ifndef _dateandtime_H
#define	_dateandtime_H

#include <xc.h>

// This defines a structure to store the date, time and sunrise timing (this structure is used throughout the project)
// The year (e.g. 2021) needs more than 8-bit (can't be stored in char since char stores only up to 8-bits),
// thus an int (which stores up to 16-bit) is used instead
// The month, date, day, hour, minute, second, sunrise_hour, sunrise_minute, sunrise_second can be sufficiently stored in 8-bits,
// thus char (which stores up to 8-bit) is used to make the code as memory-space-efficient as possible
typedef struct {
    signed int year;
    signed char month,date,day,dst,hour,minute,second,sunrise_hour,sunrise_minute,sunrise_second;
} dateandtime;

// Function prototypes
dateandtime time_incre(dateandtime current);
dateandtime daylightsavingstime_toggle(dateandtime current);
dateandtime date_check(dateandtime current);
dateandtime sunrise(dateandtime current);
dateandtime sun_sync(dateandtime current);
dateandtime sunrise_sunset(dateandtime current);

#endif	/* _dateandtime_H */