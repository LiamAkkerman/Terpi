/*
main.c
SmotBot garden control unit
*/

#include <stdio.h>
#include <time.h> 
#include <unistd.h>
#include "main.h"


//TODO move these to sperate file of settings
//define delays between the sensor readings, in seconds
const int dht22_delay = 300;
const int light_sensor_delay = 300;
const int soil_sensor_delay = 1200;




int main(int argc, char *argv[]) {
	//TODO you know, the code

}

int open_timer(int delay) {
	
	return 0;
}

int attach_handler(void) {
	
	return 0;
}
int timer_handler(int signum) {
	
	return 0;
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
