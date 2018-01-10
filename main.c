/*
main.c
SmotBot garden control unit
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "main.h"
#include "./inih/ini.h"
#include "smot_timers.h"
#include "smot_sensors.h"



configuration settings;
properties conditions;

int main(int argc, char *argv[]) {
	printf("program started\n");
	
	//read time settings from settings file
	if(ini_parse("settings.ini", ini_handler_func, &settings) < 0) {
		printf("ERROR: can't load 'settings.ini'\n");
		return -1;
    } 
	printf("settings parsed\n");
	
	measured_reset(&conditions);
	//(conditions.measured) = 0;

	//initizilize mcp3008 SPI adc
	if(startSPI(0) < 0) {
		printf("ERROR: can't start SPI\n");
		return -1;
	}
	
	//initilize timer 
	open_timer(1); // lower this for "accelerated time" for debugging
	//open_timer(FULL_INC);
	printf("timer opened\n");
	attach_handler();
	printf("timer function attached\n");
	
	
	while(1); //TODO make this not so CPU intensive to idle
	
}

int startSPI(int spiChannel) {
	printf("opening SPI channel\n");
	int result = 0;
	if((wiringPiSPISetup(spiChannel, 1000000)) < 0) {
		result = -1;
	}
	
	return result;
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


//####################&&&&&&&&&&&&&&&######################&&&&&&&&&&&&&############# bookmark
void timer_handler(int signum) {
	static unsigned int count = 0; 
	char result = 0;
	
	//if the current interval count is a prechosen time, preform approriate action
	if((count == 0) || (count == (3*FULL_DAY))) {
		int irl_count = get_irl_time(settings.increment_size);
		if(irl_count < 0) {
			printf("ERROR: system clock time failed\n");
			result = -1;
		} else {
			count = irl_count;
		}
	}
	/* TODO make more dynamic, scaling without needed to rewrite
		one possible solution is to make actions and sensors structs within an array of all of the structs.
		the structs would containing function handlers and timing values.
		this could also work in conjunction with the strings needed for posting to influx.
		*/
	if((count % settings.dht22_delay) == 0) {
		if(read_dht22(&conditions) != 0) {
			printf("ERROR: DHT22 reading failed\n");
			result = -1;
		}
	}
	if((count % settings.light_sensor_delay) == 0) {
		if(read_light(&conditions) != 0) {
			printf("ERROR: light reading failed\n");
			result = -1;
		}
	}
	if((count % settings.soil_sensor_delay) == 0) {
		if(read_soil_moist(&conditions) != 0) {
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
	if(measured_any(&conditions) > 0) {
		post_data(&conditions);
		measured_reset(&conditions);
	}
	
	printf("-- Current increment: %u --\n", count);
	count = count + 1;
	
	/* 
		TODO:
		I needed to changed this to void to work with the attach_handler function
		I don't actaully know how the referance to a function works, ...
		so I don't know if we can have this return anything or if it needs to be a void function
	//return result;
	*/
}




int measured_any(const properties *conditions_to_check) {
	//check alls properties that can be measured, returns 1 if any have been.
	
	int result = 0;
	result = conditions_to_check->temperature_measured || conditions_to_check->humidity_measured || conditions_to_check->light_measured || conditions_to_check->moisture_measured;
	return result;
}

int measured_reset(properties *conditions_to_check) {
	//force resets measured flags to false
	conditions_to_check->temperature_measured = 0;
	conditions_to_check->humidity_measured = 0;
	conditions_to_check->light_measured = 0;
	conditions_to_check->moisture_measured = 0;
	
	return 0;
}

//posting data and influxDB function
int post_data(const properties *conditions_to_post) {
	printf("posting data to influxDB\n");
	CURL *curl;
	CURLcode curl_code;
	long http_code;
	char str_result = 0;
	
	char server_url[64];
	str_result = snprintf(server_url, sizeof server_url, "http://%s/write?db=%s", settings.influx_url, settings.influx_db); 
	if(str_result < 0) {
		printf("ERROR: influx url parse overflow or error\n");
	}	
	
	char influx_auth[64];
	strcpy(influx_auth, settings.influx_auth);
	
	char message[512];
	memset(message, 0, sizeof(message));
	char single_message[128]; //might need more?
	memset(single_message, 0, sizeof(single_message));
	
	if(conditions_to_post->temperature_measured > 0) {
		str_result += snprintf(single_message, sizeof single_message, 
			"temperature,light_on=%u value=%u\n", 
			conditions_to_post->light_on, conditions_to_post->temperature);
		strcat(message, single_message);
	}
	if(conditions_to_post->humidity_measured > 0) {
		str_result += snprintf(single_message, sizeof single_message, 
			"humidity,light_on=%u value=%u\n", 
			conditions_to_post->light_on, conditions_to_post->humidity);
		strcat(message, single_message);
	}
	if(conditions_to_post->light_measured > 0) {
		str_result += snprintf(single_message, sizeof single_message, 
			"light,light_on=%u value=%u\n", 
			conditions_to_post->light_on, conditions_to_post->light_level);
		strcat(message, single_message);
	}
	if(conditions_to_post->moisture_measured > 0) {
		str_result += snprintf(single_message, sizeof single_message, 
			"moisture value=%u", 
			conditions_to_post->soil_moisture);
		strcat(message, single_message);
	}
		
	if(str_result < 0) { //this only checks the last thing but whatever... TODO I guess
		printf("ERROR: influx message errors %d\n", str_result);
		//TODO handle if buffer exceeded
	}
	
	
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, server_url);
		curl_easy_setopt(curl, CURLOPT_USERPWD, influx_auth);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
	
		curl_code = curl_easy_perform(curl);
		
		/* Check for errors */ 
		if(curl_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_code));
			printf("Server URL: %s\n",server_url);
			printf("influxDB post: %s\n", message);
		}
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
		if(curl_code != CURLE_ABORTED_BY_CALLBACK) {
			switch(http_code) {
				case 0 :
					printf("HTTP %d: curl failed.\n");
					break;
				case 204 :
					//influx sucsess, add message if you need even further debugging
					break;
				case 400 :
					printf("HTTP %d: Unacceptable request. Can occur with a Line Protocol "
						"syntax error or if a user attempts to write values to a "
						"field that previously accepted a different value type.\n", http_code);
					break;
				case 401 : 
					printf("HTTP %d: Unacceptable request. Can occur with "
					"invalid authentication credentials.\n", http_code);
					break;
				case 404 :
					printf("HTTP %d: Unacceptable request. Can occur if a user attempts to "
						"write to a database that does not exist.\n", http_code);
					break;
				case 500 :
					printf("HTTP %d: The system is overloaded or significantly impaired. Can occur if "
					"a user attempts to write to a retention policy that does not exist.\n", http_code);
					break;
				default :
					printf("HTTP %d: Unknown HTTP error.\n", http_code);
					break;
			}
			
		}
		
		curl_easy_cleanup(curl);
	}	
	return 0;
}
