/*
 * sac_sensors.h: This file contains all the functions for read the AHTS Sensors
 *
 *  Created on: 16/03/2014
 *      Author: victor suarez
 *      Co-Author: David Cuevas
 */

#include "Arduino.h"
#include "OneWire.h"

// Pin for air humdity and temperature sensor (AHTS).
#define AHTS_PIN A0

/*
 * MOISURE SMOOTHING
 */
#define MOISTURE_SMOOTHING  85

/*
 * MOISURE POWER PIN
 */
#define SOIL_MOISTURE_POWER_PIN 3

/*
 * MOISURE PIN
 */
#define MOISTURE_PIN A3

/*
 * WATER TANK PIN
 */
#define WTS_PIN A2


int DS18S20_Pin = A1; //DS18S20 Signal pin on Analogic 1

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on pin A1

/*------------Global Variables-------------*/
float moisture_calib       = 500;

float cached_temperature = 23;

float cached_humidity    = 11;

float cached_moisture    = 111;

int cached_lastWaterEvent = 0;

int  cached_water_level = 1;

int readingno=0;

#define soil_moisture_MIN 45
/*----------------------------------------*/

/*---------------Functions Definition------*/

float moisture_read ();

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
// datas is i/o parameter with [ humidity, temperature ].
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
/**
 * Read the data from moisture.
 *
 * returns the moisture data.
 */
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
/**
 * Read from the sensors of SAC System.
 */
void read_sensors (void)
{
  float air_data[2];

  readingno++;

  if (readingno % 4 == 0)
    {
      aths_read_data (&cached_humidity, &cached_temperature);
    }

  cached_water_level = (digitalRead(WTS_PIN) == LOW);

  digitalWrite(SOIL_MOISTURE_POWER_PIN, HIGH);
  cached_moisture = analogRead(MOISTURE_PIN);
  digitalWrite(SOIL_MOISTURE_POWER_PIN, LOW);
}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;

}

int getWaterFlow(){
	//TODO
}
int setWaterFlow_diameter(){
	//TODO
	int water_diameter = 16//TODO;
}
int getWaterFlow_diameter(int water_diameter){
	int waterFlow_diameter = water_diameter;
	return waterFlow_diameter;
}

#define watering_time = 60;
double getWater_consumption(int waterflow_diameter double watering_time){
	double water_consumption = waterflow_diameter * watering_time;
	return water_consumption;
}
boolean getWaterLevel(){
	//TODO
}


// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;
int hallsensor = 2;    //The pin location of the sensor

void rpm ()     //This is the function that the interupt calls
{
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
}

// the loop() method runs over and over again,
// as long as the Arduino has power
float getWaterFlowRate ()
{
  NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
  sei();      //Enables interrupts
  delay (1000);   //Wait 1 second
  cli();      //Disable interrupts
  Calc = (NbTopsFan * 7 / 60); //(Pulse frequency x 60) / 7Q, = flow rate in L/min
  return Calc;
  /*  Serial.print (Calc, DEC); //Prints the number calculated above
    Serial.print (" L/min\r\n"); //Prints "L/min" and returns a  new line  */
}
