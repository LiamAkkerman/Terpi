//main
//int main(int, char *);

//timer functions
int open_timer(int);
int attach_handler(void);
void timer_handler(int);
int get_irl_time(void);

static int ini_handler_func(void*, const char*, const char*, const char* );

//reading sensor functions
int read_dht22(void);
int read_light(void);
int read_soil_moist(void);

//posting data and influxDB functions
int post_data(void); //this'll probably have arguments when implemented


#define FULL_HOUR ((int)(60/settings.increment_size))
#define FULL_DAY (24*FULL_HOUR)
#define FULL_INC (settings.increment_size)

typedef struct {
	int increment_size; //TODO possibly change this to be compile-time constant, but still changable
	int dht22_delay;
	int light_sensor_delay;
	int soil_sensor_delay;
	int light_on_time;
	int light_on_duration;
	int exhaust_delay;
	int exhaust_duration;
	int circulation_delay;
	int circulation_duration;
} configuration;

configuration settings;

