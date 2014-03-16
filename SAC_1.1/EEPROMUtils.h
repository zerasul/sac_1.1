/*
 * EEPROMUtils.h
 *
 *  Created on: 16/03/2014
 *      Author: victor
 */
#include "EEPROM.h"
#include <stdint.h>
#define EEPROM_write(pos,val) EEPROM.write(pos,val)
#define EEPROM_read(pos)      EEPROM.read(pos)



