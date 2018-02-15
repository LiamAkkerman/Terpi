
#include <string>
#include <iostream>
#include <vector>

#include "sensor.hpp"
#include "./inih/INIReader.h"
#include "settings.hpp"

#include "sensor_vec.hpp"


Sensor_Vec::Sensor_Vec(const time_settings *t_set, const pin_settings *p_set) {
	
	auto dht_str1 = std::string("temperature");
	auto dht_temp = Sensor(t_set->dht22_delay, dht_str1, p_set->dht_pin, Sen_Type::dht22);	
	vec.push_back(&dht_temp);
	
	auto dht_str2 = std::string("humidity");
	auto dht_hum = Sensor(t_set->dht22_delay, dht_str2, p_set->dht_pin, Sen_Type::dht22);	
	vec.push_back(&dht_hum);
	
	auto light_str = std::string("light");
	auto light = Sensor(t_set->light_delay, light_str, p_set->light_pin, Sen_Type::light);	
	vec.push_back(&light);
	
	auto moist_str = std::string("moisture");
	auto moist = Sensor(t_set->soil_delay, moist_str, p_set->soil_pin, Sen_Type::soil);	
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
