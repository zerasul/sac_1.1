/*
 * sac_sensors.h: This file contains all the functions for read the AHTS Sensors
 *
 *  Created on: 16/03/2014
 *      Author: victor suarez
 *      Co-Author: David Cuevas
 */

#include "Arduino.h"

// Pin for air humdity and temperature sensor (AHTS).
#define AHTS_PIN A0

#define MOISTURE_SMOOTHING  85
/*------------Global Variables-------------*/
float moisture_calib       = 500;

/*----------------------------------------*/

// Function to read AHTS.
unsigned char read_ahts_dat() {
  unsigned char i = 0;
  unsigned char result=0;
  for(i=0; i< 8; i++) {
    int j = 0;
    while(!digitalRead(AHTS_PIN) && j++ < 500);  // wait for 50us.
    delayMicroseconds(30);

    if(digitalRead(AHTS_PIN))
      result |=(1<<(7-i));
    j = 0;
    while(digitalRead(AHTS_PIN) && j++ < 500);  // wait '1' finish.
  }
  return result;
}

// Function to process AHTS data and read it.
// datas is i/o parameter with [ humdity, temperature ].
void aths_read_data(float *hum_out, float *temp_out) {
  unsigned char ahts_dat[5];
  //unsigned char ahts_in;
  unsigned char i;
  float humdity, temperature;
  // start condition
  // 1. pull-down i/o pin from 18ms
  digitalWrite(AHTS_PIN,LOW);
  delay(18);

  digitalWrite(AHTS_PIN,HIGH);
  delayMicroseconds(40);

  pinMode(AHTS_PIN,INPUT);

  while(digitalRead(AHTS_PIN)){
    delayMicroseconds(15);
    pinMode(AHTS_PIN,OUTPUT);
    digitalWrite(AHTS_PIN,HIGH);

    return;
  }
  delayMicroseconds(80);

  while(!digitalRead(AHTS_PIN)){
    delayMicroseconds(15);
    pinMode(AHTS_PIN,OUTPUT);
    digitalWrite(AHTS_PIN,HIGH);

    return;
  }
  delayMicroseconds(80);
  // now ready for data reception
  for (i=0; i<5; i++)
    ahts_dat[i] = read_ahts_dat();

  pinMode(AHTS_PIN,OUTPUT);
  digitalWrite(AHTS_PIN,HIGH);

  unsigned char ahts_check_sum = ahts_dat[0]+ahts_dat[1]+ahts_dat[2]+ahts_dat[3];
  // check check_sum

  if(ahts_dat[4]!= ahts_check_sum)
  {
    message ("AHTS", "checksum error");
    return;
  }
  humdity=((float)(ahts_dat[0]*256+ahts_dat[1]))/10.0;

  if (ahts_dat[2] & 0x80)
    temperature=-((float)((ahts_dat[2]&0x7f)*256+ahts_dat[3]))/10.0;
  else
    temperature=((float)(ahts_dat[2]*256+ahts_dat[3]))/10.0;

  if (hum_out) *hum_out = humdity;
  if (temp_out) *temp_out = temperature;
}

float moisture_read()
{
  static float kept = 0;
  float soil_moisture = cached_moisture * 100 / moisture_calib;
  if (soil_moisture > 100)
    {
      soil_moisture = 100;
    }
  kept = (kept * MOISTURE_SMOOTHING/100.0);
  kept = kept + soil_moisture * (100-MOISTURE_SMOOTHING)/100.0;
  return kept;
}
