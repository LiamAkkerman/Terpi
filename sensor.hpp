
#ifndef SENSOR_H
#define SENSOR_H


//enum class Sen_Type {dht22, light, soil};

class Sensor {	
	protected:
		char pin;
		char channel; //for SPI or i2c
	
	public:
		unsigned int delay;
		unsigned int value;
		bool measured;
		const std::string database;
		
		virtual int measure() =0;
		
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

class Analog : public Sensor {
	protected:
		int mcpAnalogRead(int, int);
	
	public:
		Analog(unsigned int delay_in, char pin_in, const std::string& str_in) : Sensor{delay_in, pin_in, str_in} {}
		~Analog() = default;
};

class LightSen : public Analog {
	int read_light();
	
	public:
		int measure() { return read_light(); }
		
		LightSen(unsigned int delay_in, char pin_in) : Analog{delay_in, pin_in, "light"} {} 
		~LightSen() = default;
};

class MoistSen : public Analog {
	int read_moist();
	
	public:
		int measure() { return read_moist(); }
		
		MoistSen(unsigned int delay_in, char pin_in) : Analog{delay_in, pin_in, "moisture"} {} 
		~MoistSen() = default;
};

#endif
