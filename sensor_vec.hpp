#ifndef SENSOR_VEC_H
#define SENSOR_VEC_H

class Sensor_Vec {
	
	public :
		std::vector<Sensor*> vec;
		
		int reset();
		bool check_measured();
		
		Sensor_Vec(const configuration *);
};



#endif
