/*
 * RTCUtils.h
 *
 *  Created on: 16/03/2014
 *      Author: dcuevas
 */
/*
 * RTCUtils.h: This file contains all the functions for reading the Real Time Clock
 *
 *  Created on: 16/03/2014
 *      Author: David Cuevas
 *      Co-Author: victor suarez
 */

#include "Arduino.h"
#include "DS1307RTC.h"
#include "Time.h"
#include "Wire.h"



int RTCread(tmElements_t* tm){
	if (RTC.chipPresent()){
	return RTC.read(*tm);
	}
	else{
		return 0;
	}
}

int getTimeElapsed(tmElements_t tmStart, tmElements_t tmEnd){
	int time_elapsed = tmStart - tmEnd;
	return time_elapsed;
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
char* getTime(tmElements_t tm){
	char* time = "";
	time =strcpy(time, tm.Hour);
	time= strcpy(time, ":");
	time= strcpy(time, tm.Minute);
	return time;
}

char* getDate(tmElements_t tm){
	char* date = "";
	date=strcpy(date, tm.Day);
	date=strcpy(date, "/");
	date=strcpy(date, tm.Month);
	date=strcpy(date, "/");
	date= strcpy(date, tm.Year);
	return date;
}
char * getDateTime(tmElements_t tm){
	char * dateTime="";
	dateTime=strcpy(dateTime,getDate(tm));
	dateTime=strcpy(dateTime," ");
	dateTime=strcpy(dateTime,getTime(tm));
	return dateTime;
}
tmElements_t getTimeElement(char * timeDate){
	tmElements_t tm;
	char * date;
	char * time;
	char* aux=strstr(timeDate," ");
	 date= timeDate-date;

}
/*
 * Get the seconds Between two timeElements.
 *
 * Param time1: First Time
 * Param time2: Second Time
 *
 * return: Seconds Between the two dates
 */
long getSecondsBetween(tmElements_t time1,tmElements_t time2){
	long seconds1= time1.Hour*3600 + time1.Minute*60+time1.Second;
	long seconds2 = time2.Hour*3600 + time2.Minute*60+time2.Second;

	return seconds2-seconds1;
}

