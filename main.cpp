/*
main.c
Terpi garden control unit
*/

#include <iostream>
#include <string.h>
#include <cstdlib>
#include <vector>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "main.hpp"
#include "sensor.hpp"
#include "sensor_vec.hpp"
#include "./inih/ini.h"
#include "terpi_timers.hpp"
#include "influx.hpp"




configuration settings;
properties conditions;

int main(int argc, char *argv[]) {
	std::cout << "program started" << std::endl;
	
	
	auto str_test = std::string("farts");
	//auto sen_test = Sensor(60, str_test, read_dht22);
	auto sen_test = Sensor(60, str_test, 22, Sen_Type::dht22);
	
	
	
	//read time settings from settings file
	//TODO change to C++ inih
	if(ini_parse("settings.ini", ini_handler_func, &settings) < 0) {
		std::cout << "ERROR: can't load 'settings.ini'" << std::endl;
		return -1;
    } 
	std::cout << "settings parsed" << std::endl;
	
	//(conditions.measured) = 0;
	
	
	auto sensors = Sensor_Vec(&settings);
	
	sensors.vec[1]->measure();
	

	//initialize mcp3008 SPI adc
	if(startSPI(settings.spi_channel) < 0) {
		std::cout << "ERROR: can't start SPI" << std::endl;
		return -1;
	}
	
	//initialize timer 
	open_timer(1); // lower this for "accelerated time" for debugging
	//open_timer(FULL_INC);
	std::cout << "timer opened" << std::endl;
	attach_handler();
	std::cout << "timer function attached" << std::endl;
	
	
	while(1); //TODO make this not so CPU intensive to idle
	
}

int startSPI(int spi_channel) {
	std::cout << "opening SPI channel" << std::endl;
	int result = 0;
	if((wiringPiSPISetup(spi_channel, 1000000)) < 0) {
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
	else if(MATCH("System", "spi_channel")) {
		pconfig->spi_channel = atoi(value);
	}
	else if(MATCH("System", "light_pin")) {
		pconfig->light_pin = atoi(value);
	}		
	else if(MATCH("System", "soil_pin")) {
		pconfig->soil_pin = atoi(value);
	}
	else if(MATCH("System", "dht_pin")) {
		pconfig->dht_pin = atoi(value);
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
			std::cout << "ERROR: system clock time failed" << std::endl;
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
		/*if(read_dht22(&conditions) != 0) {
			std::cout << "ERROR: DHT22 reading failed" << std::endl;
			result = -1;
		} */
	}
	if((count % settings.light_sensor_delay) == 0) {
		/* if(read_light(&conditions) != 0) {
			std::cout << "ERROR: light reading failed" << std::endl;
			result = -1;
		} */
	}
	if((count % settings.soil_sensor_delay) == 0) {
		/* if(read_soil_moist(&conditions) != 0) {
			std::cout << "ERROR: soil moisture reading failed" << std::endl;
			result = -1;
		} */
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
	/* if(measured_any(&conditions) > 0) {
		post_data(&conditions);
		//measured_reset(&conditions);
	} */
	
	std::cout << "-- Current increment: " << count << " --" << std::endl;
	count = count + 1;
	
	/* 
		TODO:
		I needed to changed this to void to work with the attach_handler function
		I don't actaully know how the referance to a function works, ...
		so I don't know if we can have this return anything or if it needs to be a void function
	//return result;
	*/
}




