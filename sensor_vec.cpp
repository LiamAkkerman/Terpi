
#include <string>
#include <iostream>
#include <vector>

#include "sensor.hpp"
#include "main.hpp"
#include "sensor_vec.hpp"


Sensor_Vec::Sensor_Vec(const configuration *settings_in) {
	
	auto dht_str1 = std::string("temperature");
	auto dht_temp = Sensor(settings_in->dht22_delay, dht_str1, settings_in->dht_pin, Sen_Type::dht22);	
	vec.push_back(&dht_temp);
	
	auto dht_str2 = std::string("humidity");
	auto dht_hum = Sensor(settings_in->dht22_delay, dht_str2, settings_in->dht_pin, Sen_Type::dht22);	
	vec.push_back(&dht_hum);
	
	auto light_str = std::string("light");
	auto light = Sensor(settings_in->light_sensor_delay, light_str, settings_in->light_pin, Sen_Type::light);	
	vec.push_back(&light);
	
	auto moist_str = std::string("moisture");
	auto moist = Sensor(settings_in->soil_sensor_delay, moist_str, settings_in->soil_pin, Sen_Type::soil);	
	vec.push_back(&moist);
	
}

int Sensor_Vec::reset() {
	int result = 0;
	
	for (const auto& i : vec) {
		i->measured = 0;
	}
	
	return result;
}

bool Sensor_Vec::check_measured() {
	bool result = 0;
		
	for (const auto& i : vec) {
		result |= (i->measured);		
	}
	
	return result;
}
