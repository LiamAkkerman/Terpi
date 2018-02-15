#ifndef SENSOR_VEC_H
#define SENSOR_VEC_H

class Sensor_Vec {
	
	public :
		std::vector<Sensor*> vec; //TODO change to dynamic memory
		
		int reset();
		bool check_measured();
		//Sensor& operator[](int i) { return vec[i]; }
		
		Sensor_Vec(const time_settings *, const pin_settings *);
};


#endif
