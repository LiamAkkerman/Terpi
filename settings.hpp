
#ifndef settings_H
#define settings_H

#include "./inih/INIReader.h" //TODO this shouldn't be needed in the header


struct influx_settings {
	std::string influx_url;
	std::string influx_db;
	std::string influx_auth;
	
	int read_ini(INIReader*);
	
	influx_settings() {} 
	influx_settings(INIReader* reader) { read_ini(reader); }
};


struct time_settings {
	unsigned int dht22_delay;
	unsigned int light_delay;
	unsigned int soil_delay;
	unsigned int light_on_time;
	unsigned int light_on_duration;
	unsigned int exhaust_delay;
	unsigned int exhaust_duration;
	unsigned int circulation_delay;
	unsigned int circulation_duration;
	
	int read_ini(INIReader*);
	
	time_settings() {} 
	time_settings(INIReader* reader) { read_ini(reader); }
};
	
	
struct pin_settings {
	char spi_channel;
	char light_pin;
	char soil_pin;
	char dht_pin;
	
	int read_ini(INIReader*);
	
	pin_settings() {}
	pin_settings(INIReader* reader) { read_ini(reader); }
};

#endif
