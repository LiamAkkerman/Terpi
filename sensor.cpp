
#include <string>
#include <vector>
#include <iostream>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "sensor.hpp"

Sensor::Sensor(unsigned int delay_in, const std::string& str_in, char pin_in, Sen_Type type_in) 
	: database(str_in)  {
	delay = delay_in;
	pin = pin_in;
	type = type_in;
	value = 0;
	measured = 0;
	
	
	//measure_func = measure_func_in;
	
}

int Sensor::measure() {
	std::cout << "measuring" << std::endl;
	
	return 0;
}




