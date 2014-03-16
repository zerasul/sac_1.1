/*
 * EEPROMUtils.h: this file contains all the EEPROM Functions for use in SAC Project.
 *
 *  Created on: 16/03/2014
 *      Author: victor suarez<suarez.garcia.victor@gmail.com>
 *      Co-Author: David Cuevas Lopez
 */
#include "EEPROM.h"
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



