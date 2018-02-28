
#ifndef SENSOR_H
#define SENSOR_H


//enum class Sen_Type {dht22, light, soil};

class Sensor {	
	protected:
		int pin;
		int channel; //for SPI or i2c
		
		int add_database(const std::string&);
		
	public:
		unsigned int delay;
		bool measured;
		std::vector<float> value_vec;
		std::vector<std::string> database_vec;
		
		virtual int print(std::ostream&) const;
		virtual int measure() =0;
		
		Sensor(unsigned int, int);
		//Sensor(const Sensor&);
		~Sensor() = default;
};

class Dht22 : public Sensor {
	int read_dht22();
	
	public:
		int measure() { return read_dht22(); }
		
		Dht22(unsigned int, int);
		~Dht22() = default;
};

class Analog : public Sensor {
	protected:
		int mcpAnalogRead(int, int);
	
	public:
		Analog(unsigned int, int);
		~Analog() = default;
};

class LightSen : public Analog {
	int read_light();
	
	public:
		int measure() { return read_light(); }
		
		LightSen(unsigned int, int);
		~LightSen() = default;
};

class MoistSen : public Analog {
	int read_moist();
	
	public:
		int measure() { return read_moist(); }
		
		MoistSen(unsigned int, int);
		~MoistSen() = default;
};

#endif
