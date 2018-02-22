
#ifndef SENSOR_H
#define SENSOR_H


//TODO polymorphism

//enum class Sen_Type {dht22, light, soil};

class Sensor {
	char pin;
	char channel; //for SPI or i2c
	//Sen_Type type;
	
	
	int read_light();
	int read_soil();
	int mcpAnalogRead(int, int);
	
	public:
		unsigned int delay;
		unsigned int value;
		bool measured;
		const std::string database;
		
		virtual int measure() =0;
		
		//Sensor(unsigned int, const std::string&, char, Sen_Type);
		//Sensor(unsigned int, const std::string&, char, Sen_Type, char);
		Sensor(unsigned int, char, const std::string&);
		//Sensor(const Sensor&);
		~Sensor() = default;
};

class Dht22 : public Sensor {
	int read_dht22();
	
	public:
		int measure() { return read_dht22(); }
		
		Dht22(unsigned int delay_in, char pin_in) : Sensor{delay_in, pin_in, "temperature"} {} 
		~Dht22() = default;
};

#endif
