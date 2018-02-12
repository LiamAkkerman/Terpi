
#ifndef TERPI_SENSORS_H
#define TERPI_SENSORS_H

#include "sensor.hpp" //this feels wrong to have in the header but it made it work...

//reading sensor functions
//int read_dht22(properties *);
int read_dht22();
int read_light(properties *);
int read_soil_moist(properties *);

int mcpAnalogRead(int, int);

int populate_sensors(configuration *, std::vector<Sensor> *);

#endif
