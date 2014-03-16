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

#include "SoftwareSerial.h"
#include "serLCD.h"
#include "EEPROMUtils.h"
#include "serLCDUtils.h"
#include "sac_sensors.h"

#define RELAY_1_PIN 5
#define RELAY_2_PIN 6
#define RELAY_3_PIN 4

#define BUTTON_UP_PIN 9
#define BUTTON_ENTER_PIN 8
#define BUTTON_DOWN_PIN 7

#define LOOP_DELAY    500
#define MENU_SPEEDUP  3
#define MENU_TIMEOUT  120

#define SOIL_MOISTURE_POWER_PIN 3

#define MAX_RELAYS 4
#define MAX_LIGHTS 1

// Pin for soil moisture sensor (MOISTURE).
// http://www.seeedstudio.com/wiki/Grove_-_Moisture_Sensor
#define MOISTURE_PIN A3

// Define min and max hum for soil (MOISTURE).
#define soil_moisture_MIN 45
#define soil_moisture_MAX 80



// Pin for water tank nivel sensor (WTS).
#define WTS_PIN A2

#define CFG_MAGIC_VALUE 25 //Magic value for writting EEPROM
/* distance between datalog entries in minutes */
#define LOG_INTERVAL 60

void  message(char *line1, char *line2);

/* global variables, containing this loop iterations sensor readings */
/* Caching some initial reading for sensors */
float cached_moisture    = 111;
float cached_humidity    = 11;
float cached_temperature = 23;
int   cached_water_level = 1;


static LogEntry datalog[24*60/LOG_INTERVAL];

serLCD serialLcd(11);

int lcd_initialized=0;


//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
}
