
#ifndef TIMERS_H
#define TIMERS_H

//timer functions
int open_timer(int);
int attach_handler(void);
void timer_handler(int);
int get_irl_time(unsigned int);

#endif
