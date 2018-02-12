
#ifndef SENSOR
#define SENSOR


class Sensor {
	typedef int (*func_ptr)();
	//char flags;
	
	
	public:
		unsigned int delay;
		unsigned int value;
		bool measured;
		const std::string database;
		
		func_ptr measure_func; //function pointer to the the function that conducts the measurement
		//void (*func_ptr)() measure_func;
		
		Sensor(unsigned int, const std::string&, func_ptr);
};

#endif
