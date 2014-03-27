/*
 * Sensors.cpp
 *
 *  Created on: 20/03/2014
 *      Author: dcuevas
 */

#include <Sensors.h>
#include "sac_sensors.h"
#include "debugUtils.h"

#define soil_moisture_MIN 45 // Soil moisture min desired value
#define soil_moisture_MAX 80 //Soil Moisture Max desired value
#define soil_temperature_MIN 5;  //Min temperature for soil, triggers stop of watering mode.
#define soil_temperature_MAX 40;  //Max temperature for soil, triggers start of watering mode.
#define water_flow_volume 0; //Total of water used;

#define last_WaterEvent  0; //Victor arregla este destrozo



Sensors::Sensors(float moisture_min,float moisture_max, float temps_min,float temps_max,_FLOW_SIZE flow_diameter) {

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
    float curr_flowrate=getWaterFlowRate();
    sensors_values.cached_moisture=curr_moisture;
    sensors_values.cached_temperature=curr_temps;
	sensors_values.cached_waterlevel = getWaterLevel(); // Boolean indicates if we have water or not.
	sensors_values.cached_flowvolume+=curr_flowrate/60000;//FlowRate(L/m) to FlowRate(m3/s).

	/* set the state */

	return current_state;
}
