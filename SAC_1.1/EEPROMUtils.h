/*
 * EEPROMUtils.h: this file contains all the EEPROM Functions for use in SAC Project.
 *
 *  Created on: 16/03/2014
 *      Author: victor suarez<suarez.garcia.victor@gmail.com>
 *      Co-Author: David Cuevas Lopez
 */
#include "EEPROM.h"

#define MAX_LIGHTS 1

#define CFG_MAGIC_VALUE 25 //Magic value for writting EEPROM

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

void store_settings (ConfigItem *menu)
{
  int i;
  for (i = 0; menu[i].data; i++)
    {
      int ival = 0;
      int pos = menu[i].eeprom_pos;
      switch (menu[i].type)
        {
          case ONOFF:
          case TIME:
          case LANGUAGE:
          case ROLE:
            {
              int *val = (int*)menu[i].data;
              ival = *val;
            }
            break;
          case SOIL_CALIBRATE:
          case NUMBER:
            {
              float *sval = (float*)menu[i].data;
              ival = *sval;
            }
            break;
            break;
          default:
            break;
        }

      EEPROM_write(pos*2, ival % 256);
      EEPROM_write(pos*2+1, ival / 256);
    }
  EEPROM_write(CFG_MAGIC, CFG_MAGIC_VALUE);
}

int load_settings (ConfigItem *menu)
{
  int i;
  if (EEPROM_read(CFG_MAGIC) != CFG_MAGIC_VALUE)
    return -1;
  for (i = 0; menu[i].data; i++)
    {
        {
          int pos = menu[i].eeprom_pos;
          int ival;
          ival = EEPROM_read(pos*2) +
          EEPROM_read(pos*2+1) * 256;

          switch (menu[i].type)
            {
              case SOIL_CALIBRATE:
              case NUMBER:
                {
                  float *val = (float*)menu[i].data;
                  *val = ival;
                }
                break;
              case TIME:
              case ROLE:
              case LANGUAGE:
                {
                  int *val = (int*)menu[i].data;
                  *val = ival;
                }
                break;
            }
        }
    }
    //message ("Loaded settings.", "");
    return 0;
}

void reset_settings (ConfigItem *menu)
{
  int i;
  for (i = 0; menu[i].data; i++)
    {
       if (menu[i].type == NUMBER ||
           menu[i].type == SOIL_CALIBRATE)
         {
           float *sval = (float*)menu[i].data;
           *sval = menu[i].default_value;
         }
       else
         {
           int *ival = (int*)menu[i].data;
           *ival = menu[i].default_value;
         }
    }
}

