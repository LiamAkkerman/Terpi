
#include "./inih/INIReader.h"
#include "settings.hpp"

int influx_settings::read_ini(INIReader* reader) {
	//TODO add error handling
	influx_url = reader->Get("System", "influx_url", "localhost:8086");
	influx_db = reader->Get("System", "influx_db", "smotbot");
	influx_auth = reader->Get("System", "influx_auth", "pi:sm00t");
	
	return 0;
}

int time_settings::read_ini(INIReader* reader) {
	//TODO add error handling
	dht22_delay = reader->GetInteger("Sensors", "dht22_delay", 10);
	light_delay = reader->GetInteger("Sensors", "light_delay", 10);
	soil_delay = reader->GetInteger("Sensors", "soil_delay", 60);
	light_on_time = reader->GetInteger("Controls", "light_on_time", 0);
	light_on_duration = reader->GetInteger("Controls", "light_on_duration", 0);
	exhaust_delay = reader->GetInteger("Controls", "exhaust_delay", 0);
	exhaust_duration = reader->GetInteger("Controls", "exhaust_duration", 0);
	circulation_delay = reader->GetInteger("Controls", "circulation_delay", 0);
	circulation_duration = reader->GetInteger("Controls", "circulation_duration", 0);
	
	return 0;
}
	
int pin_settings::read_ini(INIReader* reader) {
	//TODO add error handling
	spi_channel = reader->GetInteger("System", "spi_channel", 0);
	light_pin = reader->GetInteger("System", "light_pin", 0);
	soil_pin = reader->GetInteger("System", "soil_pin", 1);
	dht_pin = reader->GetInteger("System", "dht_pin", -1);
	
	return 0;
}

