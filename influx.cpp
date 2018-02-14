
#include <iostream>
#include <string.h>
#include <vector>

#include <curl/curl.h>

#include "main.hpp"
#include "sensor.hpp"
#include "sensor_vec.hpp"
#include "influx.hpp"

extern configuration settings;

//posting data and influxDB function
int post_data(const properties *conditions_to_post) {
	std::cout << "posting data to influxDB" << std::endl;
	CURL *curl;
	CURLcode curl_code;
	long http_code;
	char str_result = 0;
	
	char server_url[64];
	str_result = snprintf(server_url, sizeof server_url, "http://%s/write?db=%s", settings.influx_url, settings.influx_db); 
	if(str_result < 0) {
		std::cout << "ERROR: influx url parse overflow or error" << std::endl;
	}	
	
	char influx_auth[64];
	strcpy(influx_auth, settings.influx_auth);
	
	char message[512];
	memset(message, 0, sizeof(message));
	char single_message[128]; //might need more?
	memset(single_message, 0, sizeof(single_message));
	
	if(conditions_to_post->temperature_measured > 0) {
		str_result += snprintf(single_message, sizeof single_message, 
			"temperature,light_on=%u value=%u\n", 
			conditions_to_post->light_on, conditions_to_post->temperature);
		strcat(message, single_message);
	}
	if(conditions_to_post->humidity_measured > 0) {
		str_result += snprintf(single_message, sizeof single_message, 
			"humidity,light_on=%u value=%u\n", 
			conditions_to_post->light_on, conditions_to_post->humidity);
		strcat(message, single_message);
	}
	if(conditions_to_post->light_measured > 0) {
		str_result += snprintf(single_message, sizeof single_message, 
			"light,light_on=%u value=%u\n", 
			conditions_to_post->light_on, conditions_to_post->light_level);
		strcat(message, single_message);
	}
	if(conditions_to_post->moisture_measured > 0) {
		str_result += snprintf(single_message, sizeof single_message, 
			"moisture value=%u", 
			conditions_to_post->soil_moisture);
		strcat(message, single_message);
	}
		
	if(str_result < 0) { //this only checks the last thing but whatever... TODO I guess
		std::cout << "ERROR: influx message errors " << str_result << std::endl;
		//TODO handle if buffer exceeded
	}
	
	
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, server_url);
		curl_easy_setopt(curl, CURLOPT_USERPWD, influx_auth);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
	
		curl_code = curl_easy_perform(curl);
		
		/* Check for errors */ 
		if(curl_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_code));
			std::cout << "Server URL: " << server_url << std::endl;
			std::cout << "influxDB post: " << message << std::endl;
		}
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
		if(curl_code != CURLE_ABORTED_BY_CALLBACK) {
			switch(http_code) {
				case 0 :
					std::cout << "HTTP " << http_code << ": curl failed." << std::endl;
					break;
				case 204 :
					//influx sucsess, add message if you need even further debugging
					break;
				case 400 :
					std::cout << "HTTP " << http_code << ": Unacceptable request. Can occur with a Line Protocol "
						"syntax error or if a user attempts to write values to a "
						"field that previously accepted a different value type." << std::endl;
					break;
				case 401 : 
					std::cout << "HTTP " << http_code << ": Unacceptable request. Can occur with "
					"invalid authentication credentials." << std::endl;
					break;
				case 404 :
					std::cout << "HTTP " << http_code << ": Unacceptable request. Can occur if a user attempts to "
						"write to a database that does not exist." << std::endl;
					break;
				case 500 :
					std::cout << "HTTP " << http_code << ": The system is overloaded or significantly impaired. Can occur if "
					"a user attempts to write to a retention policy that does not exist." << std::endl;
					break;
				default :
					std::cout << "HTTP " << http_code << ": Unknown HTTP error." << std::endl;
					break;
			}
			
		}
		
		curl_easy_cleanup(curl);
	}	
	return 0;
}
