/*
 * Sensors.cpp
 *
 *  Created on: 20/03/2014
 *      Author: dcuevas
 */

#include <Sensors.h>
#include "sac_sensors.h"
#include "debugUtils.h"

/*
	 * Default Constructor.
	 *
	 *  moisture_min: minimum Moisture for enabling the Relay.
	 *  moisture_target: Target Moisture for disabling the Relay.
	 *  temps_min: minimum Soil Temperature for enabling the Relay.
	 *  temps_max: Maximum Soil Temperature for disabling the Relay.
	 */
Sensors::Sensors(float moisture_min,float moisture_target, float temps_min,float temps_max,_FLOW_SIZE flow_diameter) {

	sensors_values.cached_maxmoisture = moisture_target;
	sensors_values.cached_minmoisture = moisture_min;

	sensors_values.cached_tempmin = temps_min;
	sensors_values.cached_tempmax = temps_max;
	sensors_values.cached_flowvolume = 0;
	sensors_values.cached_waterFlowdiameter = flow_diameter;
	sensors_values.cached_lastWaterEvent = 0;//TODO
	sensors_values.cached_fieldCapacity=TRUE;
}

Sensors::~Sensors() {
	// TODO Auto-generated destructor stub

}
/*
 * Update Current Sensors and update the Sensors data Cache
 */
void Sensors::update_sensors(void){
	 float curr_moisture=moisture_read();
	    float curr_temps= getTemp();
	    float curr_flowrate=getWaterFlowRate();
	    sensors_values.cached_moisture=curr_moisture;
	    sensors_values.cached_temperature=curr_temps;
		sensors_values.cached_waterlevel = getWaterLevel(); // Boolean indicates if we have water or not.
		sensors_values.cached_flowvolume+=curr_flowrate/60000;//FlowRate(L/m) to FlowRate(m3/s).
		sensors_values.cached_fieldCapacity=readFieldCapacity();
}
/*
 * Get The Current State
 *
 * return: Current State of the Sensors.(Get the last Update Information from Cache)
 */
State Sensors::read_sensors() {

    State current_state;


	/* set the state */
	current_state.moisture_MAX=sensors_values.cached_maxmoisture;
	current_state.moisture_MIN=sensors_values.cached_minmoisture;
	current_state.consumption=sensors_values.cached_flowvolume;
	current_state.current_temps=sensors_values.cached_temperature;
	current_state.moisture_target=sensors_values.cached_moisture;
	current_state.temps_max=sensors_values.cached_tempmax;
	current_state.temps_min=sensors_values.cached_tempmin;
	current_state.field_capacity=sensors_values.cached_fieldCapacity;
	return current_state;
}
