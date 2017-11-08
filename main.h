//main
int main(int, char *);

//timer functions
int open_timer(int);
int attach_handler(void);
int timer_handler(int);
int get_irl_time(void);

//reading sensor functions
int read_dht22(void);
int read_light(void);
int read_soil_moist(void);

//posting data and influxDB functions
int post_data(void); //this'll probably have arguments when implemented
