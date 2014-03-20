/*
 * RTCUtils.h
 *
 *  Created on: 16/03/2014
 *      Author: dcuevas
 */
/*
 * sac_sensors.h: This file contains all the functions for reading the Real Time Clock
 *
 *  Created on: 16/03/2014
 *      Author: David Cuevas
 *      Co-Author: victor suarez
 */

#include "Arduino.h"
#include "DS1307RTC.h"
#include "Time.h"
#include "Wire.h"
tmElements_t tm;


int RTCread(tmElements_t* tm){
	if (RTC.chipPresent()){
	return RTC.read(*tm);
	}
	else{
		return 0;
	}
}
int getHour(tmElements_t tm){
	return tm.Hour;
}
int getMinutes(tmElements_t tm){
	return tm.Minute;
}
int getSeconds(tmElements_t tm){
	return tm.Second;
}
int getDay(tmElements_t tm){
	return tm.Day;
}
int getMonth(tmElements_t tm){
	return tm.Month;
}
int getYear(tmElements_t tm){
	return tm.Year;
}

