/*
 * Sensors.cpp
 *
 *  Created on: 20/03/2014
 *      Author: dcuevas
 */

#include <Sensors.h>
#include "sac_sensors"

#define soil_moisture_MIN 45 // Soil moisture min desired value
#define soil_moisture_MAX 80 //Soil Moisture Max desired value
#define soil_temperature_MIN 5;  //Min temperature for soil, triggers stop of watering mode.
#define soil_temperature_MAX 40;  //Max temperature for soil, triggers start of watering mode.
#define water_flow_volume 0; //Total of water used;
#define water_flow_diameter 16; //Diameter of water entry point.For calculating water consumption.Posible Values 16,32,48 mm.

Sensors::Sensors() {
	// TODO Auto-generated constructor stub
	sensors_values.cached_minhumidity = soil_moisture_MIN;
	sensors_values.cached_maxhumidity = soil_moisture_MAX;
	sensors_values.cached_tempmin = soil_temperature_MIN;
	sensors_values.cached_tempmax = soil_temperature_MAX;
	sensors_values.cached_flowvolume = water_flow_volume;
	sensors_values.cached_waterFlowdiameter = water_flow_diameter;
}

Sensors::~Sensors() {
	// TODO Auto-generated destructor stub
}

cached_sensors Sensors::read_sensors() {
	// TODO Auto-generated constructor stub

	sensors_values.cached_humidity =  moisture_read(); //Read moisture from soil.
	sensors_values.cached_temperature = getTemp(); //Read temperature from soil.
	sensors_values.cached_waterlevel = getWaterLevel(); // Boolean indicates if we have water or not.

	/*Calculating Water consumption by multiplying watering pipe diameter with time in watering mode*/
	sensors_values.cached_flowvolume = getWater_consumption(Waterflow_diameter watering_time);

}
