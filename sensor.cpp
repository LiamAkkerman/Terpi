
#include <string>
#include <vector>
#include <iostream>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "sensor.hpp"


Sensor::Sensor(unsigned int delay_in, char pin_in, const std::string& str_in) 
	: delay{delay_in},
	pin{pin_in},
	database{str_in},
	value{0},
	measured{0} { 
		std::cout << "sensor made" << std::endl;
	}

int Dht22::read_dht22() {
	std::cout << "reading DHT22" << std::endl;
	if(!measured) {
		measured = 1;		
	}
	
	return 0;
}

int LightSen::read_light() {
	std::cout << "reading light" << std::endl;
	int light_level = 0;
	if(!measured) {
		measured = 1;
	}
	
	int spi_channel = 0;
	
	light_level = mcpAnalogRead(channel, pin);
	
	
	return light_level;
}

int MoistSen::read_moist() {
	std::cout << "reading soil moisture" << std::endl;
	int soil_moisture = 0;
	if(!measured) {
		measured = 1;
	}
	
	soil_moisture = mcpAnalogRead(channel, pin);
	
	return soil_moisture;
}

int Analog::mcpAnalogRead(int spi_channel, int analogChannel) {
	int result = 0;
	
    if(analogChannel<0 || analogChannel>7) {
		result = -1;
	} else {
		unsigned char buffer[3] = {1}; // start bit		
		buffer[1] = (analogChannel) << 4;
		wiringPiSPIDataRW(spi_channel, buffer, 3);
		//printf("buffer %02X%02X%02X\n", buffer[0], buffer[1], buffer[2]);
		result = ((buffer[1] & 3 ) << 8) + buffer[2]; // get last 10 bits
	}
	
	return result;
}


