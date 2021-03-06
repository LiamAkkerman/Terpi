
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include "sensor.hpp"
#include "settings.hpp"

#include "sensor_vec.hpp"


Sensor_Vec::Sensor_Vec(const time_settings *t_set, const pin_settings *p_set) {
	std::unique_ptr<Sensor> dht22 = std::make_unique<Dht22>(t_set->dht22_delay, p_set->dht_pin);
	vec.push_back(move(dht22));
	
	std::unique_ptr<Sensor> light_sen = std::make_unique<LightSen>(t_set->light_delay, p_set->light_pin);
	vec.push_back(move(light_sen));
	
	std::unique_ptr<Sensor> moist_sen = std::make_unique<MoistSen>(t_set->soil_delay, p_set->soil_pin);
	vec.push_back(move(moist_sen));
}


Sensor& Sensor_Vec::operator[](int i) {
	if ((i > vec.size()) ||  (i < 0)) {
		std::cout << "index " << i << " is out of bounds" << std::endl;
	} 
	return *vec[i]; 
}

int Sensor_Vec::reset_measured() {
	int result = 0;
	
	for (const auto& i : vec) {
		i->measured = false;
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
