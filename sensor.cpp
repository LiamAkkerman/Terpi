
#include <string>
#include <vector>
#include <iostream>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "sensor.hpp"

/*Sensor::Sensor(unsigned int delay_in, const std::string& str_in, char pin_in, Sen_Type type_in) 
	: database(str_in)  {
	delay = delay_in;
	pin = pin_in;
	type = type_in;
	value = 0;
	measured = 0;
	channel = 0;
	
}

//TODO maybe there's a better way than two constructors
Sensor::Sensor(unsigned int delay_in, const std::string& str_in, char pin_in, Sen_Type type_in, char channel_in) 
	: database(str_in)  {
	delay = delay_in;
	pin = pin_in;
	type = type_in;
	value = 0;
	measured = 0;
	channel = channel_in;
	
} */

Sensor::Sensor(unsigned int delay_in, char pin_in, const std::string& str_in) 
	: delay{delay_in},
	pin{pin_in},
	database{str_in},
	value{0},
	measured{0} { 
		std::cout << "sensor made" << std::endl;
	}
/*	delay = delay_in;
	pin = pin_in;
	value = 0;
	measured = 0;	
} */

	
	

/* int Sensor::measure() {
	std::cout << "measuring" << std::endl;
	int result = 0;
	
	switch(type) {
		case Sen_Type::dht22 : 
			//value = read_dht22();
			break;
		case Sen_Type::light : 
			value = read_light();
			break;
		case Sen_Type::soil : 
			value = read_soil();
			break;
	}
	
	
	return result;
} */

int Dht22::read_dht22() {
	std::cout << "reading DHT22" << std::endl;
	if(!measured) {
		measured = 1;		
	}
	
	return 0;
}

int Sensor::read_light() {
	std::cout << "reading light" << std::endl;
	int light_level = 0;
	if(!measured) {
		measured = 1;
	}
	
	int spi_channel = 0;
	
	light_level = mcpAnalogRead(channel, pin);
	
	
	return light_level;
}

int Sensor::read_soil() {
	std::cout << "reading soil moisture" << std::endl;
	int soil_moisture = 0;
	if(!measured) {
		measured = 1;
	}
	
	int spi_channel = 0;
	
	soil_moisture = mcpAnalogRead(channel, pin);
	
	return soil_moisture;
}

int Sensor::mcpAnalogRead(int spi_channel, int analogChannel) {
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


