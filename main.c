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
#include <stdlib.h>
#include <curl/curl.h>
#include "main.h"
#include "./inih/ini.h"


int main(int argc, char *argv[]) {
	printf("program started\n");
	
	//read time settings from settings file
	if(ini_parse("settings.ini", ini_handler_func, &settings) < 0) {
		printf("ERROR: can't load 'settings.ini'\n");
		return -1;
    } 
	printf("settings parsed\n");
	
	(conditions.measured) = 0;

	//initilize timer 
	open_timer(1); // lower this for "accelerated time" for debugging
	//open_timer(FULL_INC);
	printf("timer opened\n");
	attach_handler();
	printf("timer function attached\n");
	
	
	while(1); //TODO make this not so CPU intensive to idle
	
}

static int ini_handler_func(void *user, const char *section, const char *name, const char *value) {
	configuration* pconfig = (configuration*)user;

	//writes all the values from the INI to the settings struct
	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if(MATCH("Sensors", "dht22_delay")) {
		pconfig->dht22_delay = atoi(value);
	} 
	else if(MATCH("Sensors", "light_sensor_delay")) {
		pconfig->light_sensor_delay = atoi(value);
	} 
	else if(MATCH("Sensors", "soil_sensor_delay")) {
		pconfig->soil_sensor_delay = atoi(value);
	} 
	else if(MATCH("Controls", "light_on_time")) {
		pconfig->light_on_time = atoi(value);
	} 
	else if(MATCH("Controls", "light_on_duration")) {
		pconfig->light_on_duration = atoi(value);
	} 
	else if(MATCH("Controls", "exhaust_delay")) {
		pconfig->exhaust_delay = atoi(value);
	} 
	else if(MATCH("Controls", "exhaust_duration")) {
		pconfig->exhaust_duration = atoi(value);
	} 
	else if(MATCH("Controls", "circulation_delay")) {
		pconfig->circulation_delay = atoi(value);
	} 
	else if(MATCH("Controls", "circulation_duration")) {
		pconfig->circulation_duration = atoi(value);
	}
	else if(MATCH("System", "increment_size")) {
		pconfig->increment_size = atoi(value);
	}
	else if(MATCH("System", "influx_url")) {
		pconfig->influx_url = strdup(value);
	}
	else if(MATCH("System", "influx_db")) {
		pconfig->influx_db = strdup(value);
	}
	else if(MATCH("System", "influx_auth")) {
		pconfig->influx_auth = strdup(value);
	}
	else {
		return -1;  /* unknown section/name, error */
	}
	return 0;
}

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

//####################&&&&&&&&&&&&&&&######################&&&&&&&&&&&&&############# bookmark
void timer_handler(int signum) {
	static unsigned int count = 0; 
	char result = 0;
	
	//if the current interval count is a prechosen time, preform approriate action
	if((count == 0) || (count == (3*FULL_DAY))) {
		int irl_count = get_irl_time();
		if(irl_count < 0) {
			printf("ERROR: system clock time failed\n");
			result = -1;
		} else {
			count = irl_count;
		}
	}
	//TODO make more dynamic, scaling without needed to rewrite
	if((count % settings.dht22_delay) == 0) {
		if(read_dht22() != 0) {
			printf("ERROR: DHT22 reading failed\n");
			result = -1;
		}
	}
	if((count % settings.light_sensor_delay) == 0) {
		if(read_light() != 0) {
			printf("ERROR: light reading failed\n");
			result = -1;
		}
	}
	if((count % settings.soil_sensor_delay) == 0) {
		if(read_soil_moist() != 0) {
			printf("ERROR: soil moisture reading failed\n");
			result = -1;
		}
	}
	if((count % FULL_DAY) == settings.light_on_time) {
		//turn lights on
		//this is an example of a fixed time event
	}
	else if((count % FULL_DAY) == ((settings.light_on_time + settings.light_on_duration) % FULL_DAY)) {
		//turn lights off
		//this means that the duration has concluded
	}
	
	//update_display();
	if(conditions.measured) {
		post_data(&conditions);
		conditions.measured = 0;
	}
	
	printf("Current increment: %u\n", count);
	count = count + 1;
	
	/* 
		TODO:
		I needed to changed this to void to work with the attach_handler function
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
	
	//Convert to increments
	intervals = FULL_HOUR*(loc_time->tm_hour) + (int)((loc_time->tm_min)/FULL_INC);
	
	//TODO add error handling
	return intervals;
}

//reading sensor functions
int read_dht22(void) {
	printf("reading DHT22\n");
	if(!(conditions.measured)) {
		conditions.measured = 1;
	}
	
	return 0;
}
int read_light(void) {
	printf("reading light\n");
	if(!conditions.measured) {
		conditions.measured = 1;
	}
	
	return 0;
}
int read_soil_moist(void) {
	printf("reading soil moisture\n");
	if(!conditions.measured) {
		conditions.measured = 1;
	}
	
	return 0;
}

//posting data and influxDB function
int post_data(const properties *conditions_to_post) {
	printf("posting data to influxDB\n");
	CURL *curl;
	CURLcode result;
	char str_result = 0;
	
	char server_url[64];
	str_result = snprintf(server_url, sizeof server_url, "http://%s/write?db=%s", settings.influx_url, settings.influx_db); 
	if(str_result < 0) {
		printf("ERROR: influx url parse overflow or error\n");
	}
	printf("Server URL: %s\n",server_url);
	
	char message[512];
	//TODO only update condition when it's been measured
	str_result = snprintf(message, sizeof message, 
		"temperature,light_on=%u value=%u \nhumidity,light_on=%u value=%u \nlight,light_on=%u value=%u \nmoisture value=%u",
		conditions_to_post->light_on, conditions_to_post->temperature,
		conditions_to_post->light_on, conditions_to_post->humidity,	
		conditions_to_post->light_on, conditions_to_post->light_level,	
		conditions_to_post->soil_moisture);
	if(str_result < 0) {
		printf("ERROR: influx message error\n");
		//TODO handle if buffer exceeded
	}
	printf("influxDB post: %s\n", message);
	
	
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, server_url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
	
		result = curl_easy_perform(curl);
		/* Check for errors */ 
		if(result != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
		}
		
		curl_easy_cleanup(curl);
	}
	
	//TODO curl authentication
	
	
	return 0;
}
