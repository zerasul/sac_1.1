/*
 * Var.h: This file contains all the variables for the correct functionality
 * for the SAC Project.
 *
 *  Created on: 16/03/2014
 *      Author: victor Suarez<suarez.garcia.victor@gmail.com>
 *      Co-Author: David Cuevas.
 */

enum {
  S_EMPTY=0,
  S_WA,
  S_V,
  S_S,
  S_H,
  S_L,
  S_A,
  S_PU,
  S_LANGUAGE,
  S_ENGLISH,
  S_ENABLED,
  S_DISABLED,
  S_TIME,
  S_SOIL_MOISTURE,
  S_CALIBRATE_MOIST,
  S_DISCONNECTED,
  S_IRRIGATION,
  S_HEATING,
  S_COOLING,
  S_LIGHT,
  S_VENTILATION,
  S_HUMIDIFIER,
  S_ALARM,
  S_RELAY1,
  S_RELAY2,
  S_RELAY3,
  S_AIR_HUMIDITY,
  S_AIR_TEMPERATURE,
  S_IRRIGATION_CYCLE,
  S_LENGTH_SEC,
  S_RETURN_TO,
  S_MAIN_MENU,
  S_SETUP,
  S_RELAYS,
  S_RESET,
  S_ON,
  S_START,
  S_DURATION,
  S_CONFIG,
  S_RANGE,
  S_LOG,
  MIN,
};

typedef struct {
//  int temperature;
  int moisture;
//  int humidity;
} LogEntry;
