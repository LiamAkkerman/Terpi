
#ifndef SENSOR_H
#define SENSOR_H


#define FUNC_TYPE std::function<int(properties *)>

class Sensor {
	//typedef int (*func_ptr)();
	//char flags;
	
	
	public:
		unsigned int delay;
		unsigned int value;
		bool measured;
		const std::string database;
		
		FUNC_TYPE measure_func;
		//func_ptr measure_func; //function pointer to the the function that conducts the measurement
		//void (*func_ptr)() measure_func;
		
		Sensor(unsigned int, const std::string&, const FUNC_TYPE&);
};

#endif
