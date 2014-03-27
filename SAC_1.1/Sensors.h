/*
 * Sensors.h
 *
 *  Created on: 20/03/2014
 *      Author: dcuevas
 */

#ifndef SENSORS_H_
#define SENSORS_H_


/*
 * Enum Type for the Flow Size Diameter
 */
enum _FLOW_SIZE{
	/*
	 * 16 Cm Diameter
	 */
	FLOW_SIZE_16=16,
	/*
	 * 32 cm Diameter
	 */
	FLOW_SIZE_32=32,
	/*
	 * 48 cm Diameter
	 */
	FLOW_SIZE_48=48
};

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
	Sensors(float,float,float,float,_FLOW_SIZE);
	virtual ~Sensors();
	State read_sensors();
private:
	cached_sensors sensors_values;
};

#endif /* SENSORS_H_ */
