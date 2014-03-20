/*
 * debugUtils.h
 *
 *  Created on: 20/03/2014
 *      Author: victor suarez
 *  debugutils: this library is used for send data to the debug.
 */

#include "Arduino.h"
#ifndef DEBUGUTILS_H_
#define DEBUGUTILS_H_

/*
 * If DEBUGMODE is 1 The Debug Messages is Showed. If DEBUGMODe is 0 nothing happends.
 */
#define DEBUGMODE 1
/**
 * INitialize the DEBUG MOde. ONLY if DEBUGMODE is Enabled
 */
void initDebug(){

#ifdef DEBUGMODE
	Serial.begin(9600);
#endif
}

/*
 * SHow a Message in the Debug Serial.
 *
 * msage: Message to Show.
 *
 * NOTE: THis Method only works if DEBUGMODE is Enabled.
 */
void debug(const char* msage){

#ifdef DEBUGMODE
	Serial.print(msage);
#endif
}
#endif /* DEBUGUTILS_H_ */
