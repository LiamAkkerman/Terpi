
#include <iostream>
#include <vector>
#include <functional>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "main.hpp"
#include "sensor.hpp"
#include "terpi_sensors.hpp"

extern configuration settings; //TODO wow no don't use extern

/* int read_dht22() { 
	std::cout << "reading DHT22" << std::endl;
	return 0; 
}  */

//reading sensor functions
int read_dht22(properties *conditions) {
	std::cout << "reading DHT22" << std::endl;
	if(!conditions->temperature_measured) {
		conditions->temperature_measured = 1;
		conditions->humidity_measured = 1;		
	}
	
	return 0;
}

int read_light(properties *conditions) {
	std::cout << "reading light" << std::endl;
	int light_level = 0;
	if(!conditions->light_measured) {
		conditions->light_measured = 1;
	}
	
	light_level = mcpAnalogRead(settings.spi_channel, settings.light_pin);
	conditions->light_level = light_level;
	
	
	return 0;
}

int read_soil_moist(properties *conditions) {
	std::cout << "reading soil moisture" << std::endl;
	int soil_moisture = 0;
	if(!conditions->moisture_measured) {
		conditions->moisture_measured = 1;
	}
	
	
	soil_moisture = mcpAnalogRead(settings.spi_channel, settings.soil_pin);
	conditions->soil_moisture = soil_moisture;
	
	return 0;
}

int mcpAnalogRead(int spi_channel, int analogChannel) {
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



//populates a vector with all of the Sensor objects
int populate_sensors(configuration *settings_in, std::vector<Sensor> *vec_ptr_in) {
	int result = 0;
	
	auto dht_str1 = std::string("temperature");
	auto dht_temp = Sensor(settings_in->dht22_delay, dht_str1, read_dht22);	
	vec_ptr_in->push_back(dht_temp);
	
	auto dht_str2 = std::string("humidity");
	auto dht_hum = Sensor(settings_in->dht22_delay, dht_str2, read_dht22);	
	vec_ptr_in->push_back(dht_hum);
	
	auto light_str = std::string("light");
	auto light = Sensor(settings_in->light_sensor_delay, light_str, read_light);	
	vec_ptr_in->push_back(light);
	
	auto moist_str = std::string("moisture");
	auto moist = Sensor(settings_in->soil_sensor_delay, moist_str, read_soil_moist);	
	vec_ptr_in->push_back(moist);
	
	return result;
} 

