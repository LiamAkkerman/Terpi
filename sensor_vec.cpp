
#include <string>
#include <iostream>
#include <vector>

#include "sensor.hpp"
#include "settings.hpp"

#include "sensor_vec.hpp"


Sensor_Vec::Sensor_Vec(const time_settings *t_set, const pin_settings *p_set) {
	std::cout << "making vector" << std::endl;
	
	//TODO use smart pointers
	Sensor *dht22_sen = new Dht22(t_set->dht22_delay, p_set->dht_pin); //does not work with auto b/c polymorphism I think
	vec.push_back(dht22_sen);
	
	Sensor *light_sen = new LightSen(t_set->light_delay, p_set->light_pin);
	vec.push_back(light_sen);
	
	Sensor *moist_sen = new MoistSen(t_set->soil_delay, p_set->soil_pin);
	vec.push_back(moist_sen);	
}

Sensor& Sensor_Vec::operator[](int i) {
	if ((i > vec.size()) ||  (i < 0)) {
		std::cout << "index " << i << " is out of bounds" << std::endl;
	} 
	return *vec[i]; 
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
