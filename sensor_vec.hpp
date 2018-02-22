#ifndef SENSOR_VEC_H
#define SENSOR_VEC_H

class Sensor_Vec {
	
	public :
		std::vector<std::unique_ptr<Sensor>> vec; 
		
		int reset();
		bool check_measured();
		Sensor& operator[](int);
		
		Sensor_Vec(const time_settings *, const pin_settings *);
};


#endif
