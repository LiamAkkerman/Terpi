/*
main.c
SmotBot garden control unit
*/

#include <stdio.h>
#include <time.h> 
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include "main.h"


//TODO move these to sperate file of settings
//define delays between the sensor readings, in 5 minute incirments
const int dht22_delay = 2; //every 10 minutes
const int light_sensor_delay = 2;
const int soil_sensor_delay = 12; //every hour
const int light_on_time = 72; //everyday at 6am
#define FULL_DAY 288 




int main(int argc, char *argv[]) {

	open_timer(300); //default to 300 for 5 minute incriments. lower this for "accelerated time" for debugging
	attach_handler();
	
	while(1); //TODO make this not so CPU intensive to idle
	
}

//TODO change to accept prevous itimerval struct as argument
int open_timer(int delay_number) {
	struct itimerval timer;
	
	/* Configure the timer to expire after some seconds... */
	timer.it_value.tv_sec = delay_number;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = delay_number;
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

void timer_handler(int signum) {
	static int count = 0; 
	count = count + 1;
	int result = 0;
	
	//TODO possibly change to check current irl time at each interval, instead of counting intervals
	
	//if the current interval count is a prechosen time, preform approriate action
	if(count == 0) {
		int irl_count = get_irl_time();
		if(irl_count < 0) {
			printf("ERROR: system clock time failed\n");
			result = -1;
		} else {
			count = irl_count;
		}
	}
	//TODO make more dynamic, scaling without needed to rewrite
	if((count % dht22_delay) == 0) {
		if(read_dht22() != 0) {
			printf("ERROR: DHT22 reading failed\n");
			result = -1;
		}
	}
	if((count % light_sensor_delay) == 0) {
		if(read_light() != 0) {
			printf("ERROR: light reading failed\n");
			result = -1;
		}
	}
	if((count % soil_sensor_delay) == 0) {
		if(read_soil_moist() != 0) {
			printf("ERROR: soil moisture reading failed\n");
			result = -1;
		}
	}
	if((count % FULL_DAY) == light_on_time) {
		//turn lights on
		//this is an example of a fixed time event
	}
	
	/* TODO handle rollover better
		-might not even be needed if rebooted suffeciently or recalibrated regularly
		-regular recalibration could possibly miss event if time is adjusted forward
	
	if(count > 7*FULL_DAY) {
		//only do one of the following lines:
		count = get_irl_time(); //reset and recalibrate time after a week
		count = count % FULL_DAY; //reset but don't recalibrate, not really accomplishing anything
	}
	
	*/
	
	/* 
		TODO I needed to changed this to void to work with the attach_handler function
		I don't actaully know how the referance to a function works, ...
		so I don't know if we can have this return anything or if it needs to be a void function
	//return result;
	*/
}

int get_irl_time(void) {
	time_t curtime;
	struct tm *loc_time;
	int intervals;

	//Getting current time of system
	curtime = time(NULL);

	// Converting current time to local time, not seconds since Jan 1, 1970
	loc_time = localtime(&curtime);
	
	//Convert to 5 minute intervals
	intervals = 12*(loc_time->tm_hour) + (int)((loc_time->tm_min)/5);
	
	//TODO add error handling
	return intervals;
}

//reading sensor functions
int read_dht22(void) {
	
	return 0;
}
int read_light(void) {
	
	return 0;
}
int read_soil_moist(void) {
	
	return 0;
}

//posting data and influxDB function
int post_data(void) {
	
	return 0;
}
