/* SAC_1.1: New Version */
/* SAC - automated agriculture System http://sacultivo.com/

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General/ Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Written by �yvind Kol�s pippin@gimp.org in 2013, based around a core of
sensor initialization and reading by by Andres Orencio Ramirez Perez
andy@orencio.org

Rev 2.0
Adapted previous code with new SerLCD Library for using 20x4 displays, with newer functions to control
LCD. Added new Menu's and removed some unused code for clearer comprehension.

In future releases some new sensors will be added in addition to a hardware clock for simplifying all time
functions and readings.

Code adapted by David Cuevas <mr.cavern@gmail.com> with the support and collaboration of Victor Suarez <zerasul@gmail.com>
and Adrian from Artesanos Industriales del Sur.

*/

#include "SAC_1.1.h"
#include "Var.h"
#include "languages.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "serLCD.h"
#include "EEPROMUtils.h"
#include "serLCDUtils.h"
#include "sac_sensors.h"
#include "RTCUtils.h"
#include "sensors.cpp"
/*
 * RELAY PINS
 */
#define RELAY1_PIN 5
#define RELAY2_PIN 6
#define RELAY3_PIN 4

/*
 * BUTTONS PINS
 */
#define BUTTON_UP_PIN 9
#define BUTTON_ENTER_PIN 8
#define BUTTON_DOWN_PIN 7

/**
 * LOOP DELAY
 */
#define LOOP_DELAY    500

/**
 * MAX RELAYS
 */
#define MAX_RELAYS 4


// Pin for soil moisture sensor (MOISTURE).
// http://www.seeedstudio.com/wiki/Grove_-_Moisture_Sensor
#define MOISTURE_PIN A3
#define SOIL_MOISTURE_POWER_PIN 3

// Pin for water tank nivel sensor (WTS).
#define WTS_PIN A2

void  message(char *line1, char *line2, char *line3, char *line4);

/* global variables, containing this loop iterations sensor readings */
/* Caching some initial reading for sensors */


static LogEntry datalog[24*60/LOG_INTERVAL];

Relay *find_relay (int role);

Relay relay[MAX_RELAYS]={{RELAY1_PIN},{RELAY2_PIN},{RELAY3_PIN}};


//The setup function is called once at startup of the sketch
void setup_arduino()
{

	  // Initialize setup for AHTS.
	  pinMode(AHTS_PIN, OUTPUT);
	  // Initialize setup for WTS.
	  pinMode(WTS_PIN, INPUT);
	  // Initialize setup for relays.
	  pinMode(RELAY1_PIN, OUTPUT);
	  pinMode(RELAY2_PIN, OUTPUT);
	  pinMode(RELAY3_PIN, OUTPUT);

	  pinMode(SOIL_MOISTURE_POWER_PIN, OUTPUT);

	  pinMode(BUTTON_UP_PIN, INPUT);
	  pinMode(BUTTON_ENTER_PIN, INPUT);
	  pinMode(BUTTON_DOWN_PIN, INPUT);

	  //Water Flow Sensor initializing
	  pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
	  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
}

/* This key debouncer relies on being called once per loop iteration,
 * it takes the input pin and a pointer to an integer to keep track of state.
 */
static int debounce_key (int pin, int *state)
{
  int pressed = digitalRead(pin);
  int ret = 0;
  if (pressed && (*state) == 0)
    {
      ret = 1;
      *state += pressed;
    }
  else
    *state = 0;

  if (*state > 80) /* start repeating press once per cycle, after 5 cycles */
    ret=1;

  return ret;
}

int debounce_up (void)
{
  static int state = 0;
  debounce_key (BUTTON_UP_PIN, &state);
}

int debounce_down (void)
{
  static int state = 0;
  debounce_key (BUTTON_DOWN_PIN, &state);
}

int debounce_enter (void)
{
  static int state = 0;
  debounce_key (BUTTON_ENTER_PIN, &state);
}

int get_event (void)
{
  if (debounce_up ())
    return BUTTON_UP;
  if (debounce_down ())
    return BUTTON_DOWN;
  if (debounce_enter ())
    return BUTTON_ENTER;
  return IDLE;
}

Relay *find_relay (int role)
{
  Relay *ret = (Relay*)NULL;
  int i;
  for (i = 0; i < MAX_RELAYS; i++)
    if (relay[i].role == role)
      return &relay[i];
  return ret;
}

void relay_on (Relay *relay)
{
  if (relay->state == RELAY_ON)
    return;
  relay->state = RELAY_ON;


  if (relay->gpio_pin)
    digitalWrite(relay->gpio_pin, HIGH);

}

void relay_wait (Relay *relay)
{
  if (relay->state == RELAY_WAITING)
    return;
  relay->state = RELAY_WAITING;

  digitalWrite(relay->gpio_pin, LOW);

}

void relay_off (Relay *relay)
{
  if (relay->state == RELAY_OFF)
    return;
  relay->state = RELAY_OFF;

  digitalWrite(relay->gpio_pin, LOW);

}
float elapsed_minutes(int time1, int time2);

void update_relay_state (void)
{
  Relay *water       = find_relay (IRRIGATION);
  Relay *light       = find_relay (LIGHT);
  Relay *ventilation = find_relay (VENTILATION);
  Relay *humidifier  = find_relay (HUMIDIFIER);
  Relay *alarm       = find_relay (ALARM);
  Relay *heating     = find_relay (HEATING);
  Relay *cooling     = find_relay (COOLING);

  static int last_water_event = 0; /* time of day we last switch water
                                      on or off for cycling */
  int alarmed = 0;
  /* This is the core logic of SAC */
  int i;
  for (i = 0; i < MAX_RELAYS; i++)
    if (relay[i].role == DISCONNECTED)
      relay_off (&relay[i]);

  if (cooling)
    {
      int   is_cooling = cooling->state != RELAY_OFF;

      if (is_cooling)
        {
          if (cached_temperature < temperature_target - temperature_range/2)
            relay_off (cooling);
        }
      else
        {
          if (cached_temperature > temperature_target + temperature_range/2)
            relay_on (cooling);
        }
    }

  if (heating)
    {
      int   is_heating = heating->state != RELAY_OFF;

      if (is_heating)
        {
          if (cached_temperature > temperature_target + temperature_range/2)
            relay_off (heating);
        }
      else
        {
          if (cached_temperature < temperature_target - temperature_range/2)
            relay_on (heating);
        }
    }



  if (ventilation)
    {
      int   is_venting = ventilation->state != RELAY_OFF;

      if (is_venting)
        {
          if (cached_humidity < humidity_target - humidity_range/2)
            relay_off (ventilation);
        }
      else
        {
          if (cached_humidity > humidity_target + humidity_range/2)
            relay_on (ventilation);
        }
    }

  if (humidifier)
    {
      int   is_humidifing  = humidifier->state != RELAY_OFF;

      if (is_humidifing)
        {
          if (cached_humidity > humidity_target + humidity_range/2)
            relay_off (humidifier);
        }
      else
        {
          if (cached_humidity < humidity_target - humidity_range/2)
            relay_on (humidifier);
        }
    }

  if (light)
    {
      int i;
      int found = 0;
      int minutes = get_minutes_since_midnight ();

      for (i = 0; i < MAX_LIGHTS; i++)
        {
          int start;
          int end;
          start = lights_start[i];
          end   = start + lights_duration [i];

          while (end > 24 * 60)
            end -= 24 * 60;

          if (end < start)
            {
              /* time period crossing midnight */
              if ((minutes >= start && minutes < MIDNIGHT) ||
                  minutes < end)
                found = 1;
            }
          else
            {
              if (minutes >= start && minutes < end)
                found = 1;
            }

        }
      /* check if current time is inside
       * the timespan
       */
      if (found)
        relay_on (light);
      else
        relay_off (light);
    }

  if (water && !cached_water_level)
    {
      relay_off (water);
      alarmed ++;
    }
  else if (water)
    {
      float moisture = moisture_read ();
      int   is_watering = water->state != RELAY_OFF;

      if (is_watering)
        {
          if (water->state == RELAY_ON)
            {
                if (elapsed_minutes (get_seconds_since_midnight (),
                                  last_water_event) >
                    (pump_cycle_length * pump_duty_cycle / 100))
                  {
                    relay_wait (water);
                    last_water_event = get_seconds_since_midnight ();
                  }
            }
          else
            {
                if (elapsed_minutes (get_seconds_since_midnight (),
                                  last_water_event) >
                    (((100-pump_duty_cycle) * pump_cycle_length) / 100))
                  {
                    relay_on (water);
                    last_water_event = get_seconds_since_midnight ();
                  }
            }

          //if (moisture > moisture_target + moisture_range/2)
          if (moisture == soil_moisture_MAX)
            relay_off (water);
        }
      else
        {
          //if (moisture < moisture_target - moisture_range/2)
    	  if (moisture < soil_moisture_MIN)
            relay_on (water);
          last_water_event = get_minutes_since_midnight ();
        }
    }
  if (alarm)
    {
      if (alarmed)
        relay_on (alarm);
      else
        relay_off (alarm);
    }


  for (i = 0; i < MAX_RELAYS; i++)
    {
      if (relay[i].role == ON)
        relay_on (&relay[i]);
      else if (relay[i].role == DISCONNECTED)
        relay_off (&relay[i]);
    }
}

void store_log (void)
{
  /* we continously update the "current" entry */
  int logno = get_minutes_since_midnight () / LOG_INTERVAL;
  if ((logno < 0) ||
      (logno >= (24*60/LOG_INTERVAL)))
    logno = 0;
//  datalog[logno].temperature = cached_temperature;
//  datalog[logno].humidity = cached_humidity;
//  datalog[logno].moisture = moisture_read();//cached_moisture * 100 / moisture_calib;
}

void setup(){

	reset_settings (config);
	  if (load_settings (config) != 0)
	    {
	      menu_active = 10; /* XXX: should be language selection!! */
	      is_editing = 1;
	    }

	  setup_arduino ();

}
// The loop function is called in an endless loop
void loop()
{
	  read_sensors ();
	  store_log ();
	  update_relay_state ();

	  handle_events (); /* handle user events, if any */

	  draw_ui (cached_temperature,cached_humidity);       /* draw ui in current state*/

	  /* XXX
	  if (menu == main_menu && menu_active == 0)
	    delay (LOOP_DELAY);
	  else
	   */ {
	      int i;
	      for (i = 0; i < MENU_SPEEDUP; i++)
	        {
	          delay (LOOP_DELAY/MENU_SPEEDUP);
	          handle_events (); /* handle user events, if any */
	          draw_ui (cached_temperature,cached_humidity);       /* draw ui in current state*/
	        }
	    }
}
