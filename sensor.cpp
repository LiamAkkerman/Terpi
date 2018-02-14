
#include <string>
#include <functional>
#include "main.hpp" //for properties struct declaration
#include "sensor.hpp"

Sensor::Sensor(unsigned int delay_in, const std::string& str_in, const FUNC_TYPE& measure_func_in) 
	: database(str_in) {
	delay = delay_in;
	value = 0;
	measured = 0;
	
	measure_func = measure_func_in;
	
}
