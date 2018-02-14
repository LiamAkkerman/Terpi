
#ifndef SENSOR_H
#define SENSOR_H


//#define FUNC_TYPE std::function<int(properties *)>

enum class Sen_Type {dht22, analog};

class Sensor {
	char pin;
	Sen_Type type;
	
	int mcpAnalogRead(int, int);
	
	public:
		
		unsigned int delay;
		unsigned int value;
		bool measured;
		const std::string database;
		
		//FUNC_TYPE measure_func;
		int measure();
		
		Sensor(unsigned int, const std::string&, char, Sen_Type);
		//Sensor(const Sensor&);
};

#endif
