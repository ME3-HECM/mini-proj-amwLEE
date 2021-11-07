// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _dateandtime_H
#define	_dateandtime_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Initialise what day it is when device is first switched on (using manual input)
struct dateandtime;
struct dateandtime {
    unsigned int year;
    unsigned char month,date,day,hour,minute,second,sunrise_hour,sunrise_minute,sunrise_second;
};
extern struct dateandtime current;

struct dateandtime time_incre(struct dateandtime current);
struct dateandtime daylightsavingstime_toggle(struct dateandtime current);
struct dateandtime date_check(struct dateandtime current);
struct dateandtime sunrise(struct dateandtime current);
struct dateandtime sun_sync(struct dateandtime current);

#endif	/* _dateandtime_H */