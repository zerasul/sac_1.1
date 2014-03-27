/*
 * Sensors.h
 *
 *  Created on: 20/03/2014
 *      Author: dcuevas
 */

#ifndef SENSORS_H_
#define SENSORS_H_

// Define min and max hum for soil (MOISTURE).

typedef struct {
  int cached_temperature;
  float cached_humidity;
  int cached_waterlevel;
  float cached_flowvolume; //Caudal de agua
  float cached_tempmin;
  float cached_tempmax;
  float cached_moisture;
  float cached_minmoisture;
  float cached_maxmoisture;
  int cached_waterFlowdiameter;
  char * cached_lastWaterEvent;
} cached_sensors;
/*
 * This Struct store all the information for the current state.
 */
typedef struct {
	/*
	 * current time with format HH:MM:SS
	 */
	char * time;
	/*
	 * current date with format DD/MM/YYYY
	 */
	char * date;
	/**
	 * the moisture target
	 */
	float moisture_target;
	/**
	 * current moisture_MIN
	 */
	float moisture_MIN;
	/**
	 * current moisture_MIN
	 */
	float moisture_MAX;
	/**
	 * current moisture
	 */
	float current_mositure;
	/*
	 * Max Soil Temperature
	 */
	float temps_max;
	/*
	 * Min Soil Temperature
	 */
	float temps_min;
	/*
	 * Current Soil Temperature
	 */
	float current_temps;
	/*
	 * Current Water Consumption in m3
	 */
	float consumption;

}State;

class Sensors {
public:
	Sensors(float,float,float,float,float);
	virtual ~Sensors();
	State read_sensors();
private:
	cached_sensors sensors_values;
};

#endif /* SENSORS_H_ */
