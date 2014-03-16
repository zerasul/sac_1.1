/*
 * EEPROMUtils.h: this file contains all the EEPROM Functions for use in SAC Project.
 *
 *  Created on: 16/03/2014
 *      Author: victor suarez<suarez.garcia.victor@gmail.com>
 *      Co-Author: David Cuevas Lopez
 */
#include "EEPROM.h"

#define MAX_LIGHTS 1
int lights_start[MAX_LIGHTS] = {0,};
int lights_duration[MAX_LIGHTS]   = {0,};


/*Write in The EEPROM MEmory.
 *  Pos: Position in EEPROM Memory.
 *  val: Value to Store.
 */
#define EEPROM_write(pos,val) EEPROM.write(pos,val)
/**
 * Read From EEPROM Memory.
 * Pos: Position in EEPROM Memory.
 */
#define EEPROM_read(pos)      EEPROM.read(pos)


/* This enum specifies storage locations in EEPROM memory of the arduino if
 * these change; existing on-board configurations are not valid; please
 * change the magic if that happens to ensure old invalid settings are not
 * used.
 */
enum {
  CFG_MAGIC = 0,
  CFG_RELAY1,
  CFG_RELAY2,
  CFG_RELAY3,
  CFG_LANGUAGE,
  CFG_MOISTURE = 10,
  CFG_MOISTURE_MOISTUREOOTH,
  CFG_MOISTURE_CALIBRATION,
  CFG_IRRIGATION_CYCLE_LENGTH,
  CFG_IRRIGATION_DUTY,
  CFG_HUMIDITY = 20,
  CFG_TEMPERATURE = 30,
  CFG_LIGHT_A1S = 40, CFG_LIGHT_A1E,
};

/* XXX: fixme: add default */
ConfigItem config[]={
  {&relay[0].role,      ROLE,           CFG_RELAY1,                  IRRIGATION},
  {&relay[1].role,      ROLE,           CFG_RELAY2,                  ALARM},
  {&relay[2].role,      ROLE,           CFG_RELAY3,                  LIGHT},
  {&moisture_target,    NUMBER,         CFG_MOISTURE,                42},
  {&humidity_target,    NUMBER,         CFG_HUMIDITY,                78},
  {&temperature_target, NUMBER,         CFG_TEMPERATURE,             23},
  {&moisture_calib,     SOIL_CALIBRATE, CFG_MOISTURE_CALIBRATION,    500.0},
  {&pump_cycle_length,  NUMBER,         CFG_IRRIGATION_CYCLE_LENGTH, 4},
  {&lights_start[0],    TIME,           CFG_LIGHT_A1S,               0},
  {&lights_duration[0], TIME,           CFG_LIGHT_A1E,               0},
  {&active_language,    LANGUAGE,       CFG_LANGUAGE,                0},
  {NULL,}
};



