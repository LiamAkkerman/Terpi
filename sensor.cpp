
#include <string>
#include "sensor.hpp"

Sensor::Sensor(unsigned int delay_in, const std::string& str_in, func_ptr measure_func_in) 
	: database(str_in) {
	delay = delay_in;
	value = 0;
	measured = 0;
	
	measure_func = measure_func_in;
	
}
