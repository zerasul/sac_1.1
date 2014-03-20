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
  int cached_flowvolume; //Caudal de agua
  int cached_tempmin;
  int cached_tempmax;
  int cached_moisture;
  int cached_minhumidity;
  int cached_maxhumidity;
  int cached_waterFlowdiameter;
} cached_sensors;


class Sensors {
public:
	Sensors();
	virtual ~Sensors();
	cached_sensors read_sensors();
private:
	cached_sensors sensors_values;
};

#endif /* SENSORS_H_ */
