
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

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
	int light_level = 0;
	if(!conditions->light_measured) {
		conditions->light_measured = 1;
	}
	
	light_level = mcpAnalogRead(0,0);
	conditions->light_level = light_level;
	
	
	return 0;
}

int read_soil_moist(properties *conditions) {
	printf("reading soil moisture\n");
	int soil_moisture = 0;
	if(!conditions->moisture_measured) {
		conditions->moisture_measured = 1;
	}
	
	soil_moisture = mcpAnalogRead(0,1);
	conditions->soil_moisture = soil_moisture;
	
	return 0;
}

int mcpAnalogRead(int spiChannel, int analogChannel) {
	int result = 0;
	
    if(analogChannel<0 || analogChannel>7) {
		result = -1;
	} else {
		unsigned char buffer[3] = {1}; // start bit		
		buffer[1] = (analogChannel) << 4;
		wiringPiSPIDataRW(spiChannel, buffer, 3);
		//printf("buffer %02X%02X%02X\n", buffer[0], buffer[1], buffer[2]);
		result = ((buffer[1] & 3 ) << 8) + buffer[2]; // get last 10 bits
	}
	
	return result;
}

