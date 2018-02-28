//main

#ifndef MAIN_H
#define MAIN_H

int startSPI(int);

int ini_sys_settings(INIReader*, influx_settings*);

void timer_handler(int);


#define FULL_HOUR ((int)(60/settings.increment_size))
#define FULL_DAY (24*FULL_HOUR)
#define FULL_INC (settings.increment_size)

#endif

