
#include <string>
#include <vector>
#include <iostream>
#include <vector>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "sensor.hpp"


Sensor::Sensor(unsigned int delay_in, int pin_in) : 
	delay{delay_in},
	pin{pin_in},
	measured{false} {
}

Analog::Analog(unsigned int delay_in, int pin_in) :
	Sensor{delay_in, pin_in} {
}

LightSen::LightSen(unsigned int delay_in, int pin_in) :
	Analog{delay_in, pin_in} {
	
	add_database("light");
} 

MoistSen::MoistSen(unsigned int delay_in, int pin_in) :
	Analog{delay_in, pin_in} {
	
	add_database("moisture");
} 

Dht22::Dht22(unsigned int delay_in, int pin_in) :
	Sensor{delay_in, pin_in} {
	
	add_database("temperature");
	add_database("humidity");
}


int Sensor::print(std::ostream& stream) const{
	stream << "Sensor Print; pin: " << pin << ", delay: " << delay << ", measured: " << measured << std::endl; //"\n";
	for(int i = 0; i < database_vec.size(); i++) {
		stream << "    " << database_vec[i] << ":  " << value_vec[i] << "\n";
	}
	stream << "\n";
	
	return 0;
}

int Sensor::add_database(const std::string& str_in) {
	value_vec.push_back(float(0));
	database_vec.push_back(str_in);
	
	return 0;
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


