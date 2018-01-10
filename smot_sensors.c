
#include <stdio.h>
#include <unistd.h>

#include "main.h"
#include "smot_sensors.h"


//reading sensor functions
int read_dht22(properties *conditions) {
	printf("reading DHT22\n");
	if(!conditions->temperature_measured) {
		conditions->temperature_measured = 1;
		conditions->humidity_measured = 1;		
	}
	
	return 0;
}

int read_light(properties *conditions) {
	printf("reading light\n");
	if(!conditions->light_measured) {
		conditions->light_measured = 1;
	}
	
	return 0;
}

int read_soil_moist(properties *conditions) {
	printf("reading soil moisture\n");
	if(!conditions->moisture_measured) {
		conditions->moisture_measured = 1;
	}
	
	return 0;
}
