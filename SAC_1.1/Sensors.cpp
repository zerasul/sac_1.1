/*
 * Sensors.cpp
 *
 *  Created on: 20/03/2014
 *      Author: dcuevas
 */

#include <Sensors.h>
#include "sac_sensors.h"
#include "debugUtils.h"

Sensors::Sensors(float moisture_min,float moisture_max, float temps_min,float temps_max,float flow_diameter) {

	sensors_values.cached_maxmoisture = moisture_min;
	sensors_values.cached_minmoisture = moisture_max;

	sensors_values.cached_tempmin = temps_min;
	sensors_values.cached_tempmax = temps_max;
	sensors_values.cached_flowvolume = 0;
	sensors_values.cached_waterFlowdiameter = flow_diameter;
	sensors_values.cached_lastWaterEvent = 0;//TODO
}

Sensors::~Sensors() {
	// TODO Auto-generated destructor stub

}

State Sensors::read_sensors() {

    State current_state;
    float curr_moisture=moisture_read();
    float curr_temps= getTemp();
    sensors_values.cached_moisture=curr_moisture;
    sensors_values.cached_temperature=curr_temps;
	sensors_values.cached_waterlevel = getWaterLevel(); // Boolean indicates if we have water or not.

	/*Calculating Water consumption by multiplying watering pipe diameter with time in watering mode*/

	sensors_values.cached_flowvolume = getWater_consumption(Waterflow_diameter, watering_time);

	/* set the state */

	return current_state;
}
