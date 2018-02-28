
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <curl/curl.h>

#include "settings.hpp"
#include "sensor.hpp"
#include "sensor_vec.hpp"
#include "influx.hpp"


//posting data and influxDB function
int post_data(const Sensor_Vec *vec_obj, const influx_settings* settings) {
	std::cout << "posting data to influxDB" << std::endl;
	CURL *curl;
	CURLcode curl_code;
	long http_code;
	char str_result = 0;
	
	auto server_url = std::string();
	server_url = "http://" + settings->influx_url + "/write?db=" + settings->influx_db; 
	
	auto influx_auth = settings->influx_auth;
	
	auto message = std::string();
	
	for (const auto& i : vec_obj->vec) {
		if(i->measured == 1) {
			for(int j = 0; j < i->database_vec.size(); j++) {
				message += i->database_vec[j] + ", value=" + std::to_string(i->value_vec[j]) + "\n";
			}
		}
	}
	
	std::cout << message << std::endl;

	
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
