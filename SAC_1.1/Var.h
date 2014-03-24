/*
 * Var.h: This file contains all the variables for the correct functionality
 * for the SAC Project.
 *
 *  Created on: 16/03/2014
 *      Author: victor Suarez<suarez.garcia.victor@gmail.com>
 *      Co-Author: David Cuevas.
 */
float pump_duty_cycle      = 50;
float pump_cycle_length    = 0;
float temperature_target      = 0;
float temperature_range       = 1;
float moisture_target      = 0;
float moisture_range       = 10;//candidato a ser destruido


float humidity_target = 0;
float humidity_range = 10;

int   relay1_role = 0;
int   relay2_role = 0;
int   relay3_role = 0;
int   is_editing  = 0;
int   is_print    = 0;



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

enum {
  DISCONNECTED,
  IRRIGATION,
  HUMIDIFIER,
  VENTILATION,
  HEATING,
  COOLING,
    LIGHT,
  ALARM,
  ON
};

int roles[] = {
  S_DISCONNECTED,
  S_IRRIGATION,
  S_HUMIDIFIER,
  S_VENTILATION,
  S_HEATING,
  S_COOLING,
  S_LIGHT,


/*  S_ALARM,*/
/*  S_ON*/
};
