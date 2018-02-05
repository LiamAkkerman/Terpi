//main
//TODO IFNDEF thing

typedef struct {
	unsigned int dht22_delay;
	unsigned int light_sensor_delay;
	unsigned int soil_sensor_delay;
	unsigned int light_on_time;
	unsigned int light_on_duration;
	unsigned int exhaust_delay;
	unsigned int exhaust_duration;
	unsigned int circulation_delay;
	unsigned int circulation_duration;
	unsigned int increment_size; //TODO possibly change this to be compile-time constant
	const char *influx_url; //TODO memory size for this
	const char *influx_db;
	const char *influx_auth;
	char spi_channel;
	char light_pin;
	char soil_pin;
	char dht_pin;
} configuration;

typedef struct {
	unsigned int temperature:12; //maybe in kelvin, and x10 (eg 37 deg celsius == 3071)
	unsigned int humidity:8;
	unsigned int light_level:12;
	unsigned int soil_moisture:12;
	unsigned int light_on:1; //TODO maybe use stdbool.h or _Bool instead of bitfield
	unsigned int view_light_on:1;
	unsigned int exhaust_on:1;
	unsigned int fan_on:1;
	unsigned int temperature_measured:1;
	unsigned int humidity_measured:1;
	unsigned int light_measured:1;
	unsigned int moisture_measured:1;
} properties; //this bitfeild is 52 bits, ie not optimized

int startSPI(int);

static int ini_handler_func(void *, const char *, const char *, const char *);

void timer_handler(int);

//posting data and influxDB functions
int measured_any(const properties *);
int measured_reset(properties *);
int post_data(const properties *);


#define FULL_HOUR ((int)(60/settings.increment_size))
#define FULL_DAY (24*FULL_HOUR)
#define FULL_INC (settings.increment_size)



