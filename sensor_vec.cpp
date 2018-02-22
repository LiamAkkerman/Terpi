
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include "sensor.hpp"
#include "settings.hpp"

#include "sensor_vec.hpp"


Sensor_Vec::Sensor_Vec(const time_settings *t_set, const pin_settings *p_set) {
	#define dht_temp_str "temperature"
	#define dht_humd_str "humidity"
	
	std::unique_ptr<Sensor> dht22_temp = std::make_unique<Dht22>(t_set->dht22_delay, p_set->dht_pin, dht_temp_str);
	vec.push_back(move(dht22_temp));
	
	std::unique_ptr<Sensor> dht22_humd = std::make_unique<Dht22>(t_set->dht22_delay, p_set->dht_pin, dht_humd_str);
	vec.push_back(move(dht22_humd));
	
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
