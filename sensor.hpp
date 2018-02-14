
#ifndef SENSOR_H
#define SENSOR_H


//#define FUNC_TYPE std::function<int(properties *)>

enum class Sen_Type {dht22, light, soil};

class Sensor {
	char pin;
	char channel; //for SPI or i2c
	Sen_Type type;
	
	int read_dht22();
	int read_light();
	int read_soil();
	int mcpAnalogRead(int, int);
	
	public:		
		unsigned int delay;
		unsigned int value;
		bool measured;
		const std::string database;
		
		int measure();
		
		Sensor(unsigned int, const std::string&, char, Sen_Type);
		Sensor(unsigned int, const std::string&, char, Sen_Type, char);
		//Sensor(const Sensor&);
};

#endif
