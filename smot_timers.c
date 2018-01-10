
#include <time.h> 
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>

#include "smot_timers.h"



//TODO change to accept prevous itimerval struct as argument
int open_timer(int delay_number) {
	struct itimerval timer;
	
	//TODO change 10 back to 60. 10 is for super fast
	/* Configure the timer to expire after some minutes... */
	timer.it_value.tv_sec = 10*delay_number;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 10*delay_number;
	timer.it_interval.tv_usec = 0;
	
	/* Start a virtual timer. It counts down whenever this process is executing. */
	if(setitimer(ITIMER_VIRTUAL, &timer, NULL) != 0) {
		printf("ERROR: opening timer failed\n");
		return -1; //TODO I'd rather have only one final return
	}
	
	return 0;
}

//TODO change to accept prevous sigaction struct as argument
int attach_handler(void) {
	//assigns an action to occur when timer expires
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa)); //I don't actaully know what this does, but it's supposed to be used
	
	//calls the referanced function
	sa.sa_handler = &timer_handler;
	if(sigaction(SIGVTALRM, &sa, NULL) != 0) { 
		printf("ERROR: handler failed\n");
		return -1; //TODO I'd rather have only one final return
	}
	
	return 0;
}

int get_irl_time(unsigned int increment_size) {
	time_t curtime;
	struct tm *loc_time;
	int intervals;

	//Getting current time of system
	curtime = time(NULL);

	// Converting current time to local time, not seconds since Jan 1, 1970
	loc_time = localtime(&curtime);
	
	//Convert to increments
	intervals = ((int)(60/increment_size))*(loc_time->tm_hour) + (int)((loc_time->tm_min)/increment_size);
	
	//TODO add error handling
	return intervals;
}
