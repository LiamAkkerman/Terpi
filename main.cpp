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
#include "./inih/INIReader.h"

#include "settings.hpp"
#include "sensor.hpp"
#include "sensor_vec.hpp"
#include "terpi_timers.hpp"
#include "influx.hpp"
#include "main.hpp"


auto i_set = influx_settings(); //TODO make not global

int main(int argc, char *argv[]) {
	std::cout << "program started" << std::endl;
	
	
	
	//read time settings from settings file
	auto reader = INIReader("settings.ini");
	if (reader.ParseError() < 0) {
        std::cout << "ERROR: can't load 'settings.ini'" << std::endl;
        return -1;
    }
	
	auto t_set = time_settings(&reader);
	auto p_set = pin_settings(&reader);
	i_set.read_ini(&reader);
	
	std::cout << "settings parsed" << std::endl;
	
	
	auto sensors = Sensor_Vec(&t_set, &p_set);
	std::cout << "vector made" << std::endl;
	sensors[0].measure();
	
	
	
	//initialize mcp3008 SPI adc
	if(startSPI(p_set.spi_channel) < 0) {
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


void timer_handler(int signum) {
	static unsigned int count = 0; 
	char result = 0;
	
	//if the current interval count is a prechosen time, preform approriate action
	if((count == 0) || (count == (3*280))) { //TODO phony numbers
		int irl_count = get_irl_time(1); //TODO phony numbers
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
	/* if((count % settings.dht22_delay) == 0) {
		if(read_dht22(&conditions) != 0) {
			std::cout << "ERROR: DHT22 reading failed" << std::endl;
			result = -1;
		} 
	}
	if((count % settings.light_sensor_delay) == 0) {
		if(read_light(&conditions) != 0) {
			std::cout << "ERROR: light reading failed" << std::endl;
			result = -1;
		} 
	}
	if((count % settings.soil_sensor_delay) == 0) {
		 if(read_soil_moist(&conditions) != 0) {
			std::cout << "ERROR: soil moisture reading failed" << std::endl;
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
	} */
	
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
		I don't actually know how the reference to a function works, ...
		so I don't know if we can have this return anything or if it needs to be a void function
	//return result;
	*/
}




