/*
 * serLCDUtils.h
 *
 *  Created on: 16/03/2014
 *      Author: dcuevas
 */
#include "SerLCD.h"
#include "RTCUtils.h"
#include "sac_sensors.h"
#include "Var.h"

#define LCD_PIN 11

#define MENU_SPEEDUP  3
#define MENU_TIMEOUT  120

#define LCD_ROWS 4
#define LCD_COLS 20

/* distance between datalog entries in minutes */
#define LOG_INTERVAL 60
SoftwareSerial SSerial(0,LCD_PIN);
SerLCD lcd(SSerial,LCD_ROWS,LCD_COLS);

//-------------------------------------------------------------------------

#define MAX_LIGHTS 1

int lights_start[MAX_LIGHTS] = { 0, };
int lights_duration[MAX_LIGHTS] = { 0, };



/*Menus*/

MenuItem relay_menu[] = { { S_RELAY1, S_EMPTY, ROLE, &relay[0].role }, {
		S_RELAY2, S_EMPTY, ROLE, &relay[1].role }, { S_RELAY3, S_EMPTY, ROLE,
		&relay[2].role }, { S_RETURN_TO, S_MAIN_MENU, BACK, 0 }, { 0 } };

int minutes = 0;

#define MAX_MENU_DEPTH 4

MenuItem *prev_menu[MAX_MENU_DEPTH];
int prev_menu_active[MAX_MENU_DEPTH];
int menu_depth = 0;

MenuItem log_menu[] = { { S_LOG, 0, LOG, },
		{ S_RETURN_TO, S_MAIN_MENU, BACK, 0 }, { 0 } };

MenuItem main_menu[] = { { S_A, 0, STATUS, (void*) 0 }, { S_RELAYS, S_CONFIG,
		SUBMENU, &relay_menu },
		{ S_SOIL_MOISTURE, 0, NUMBER, &moisture_target }, { S_IRRIGATION_CYCLE,
				S_LENGTH_SEC, NUMBER, &pump_cycle_length }, { S_CALIBRATE_MOIST,
				0, SOIL_CALIBRATE, &moisture_calib }, { S_AIR_HUMIDITY, 0,
				NUMBER, &humidity_target }, { S_AIR_TEMPERATURE, 0, NUMBER,
				&temperature_target }, { S_LIGHT, S_START, TIME,
				&lights_start[0] }, { S_LIGHT, S_DURATION, TIME,
				&lights_duration[0] }, { S_TIME, 0, TIME, &minutes }, {
				S_LANGUAGE, 0, LANGUAGE, &active_language },
		/*{S_RESET, S_CONFIG,                RESET_SETTINGS,      0},*/
		/*{S_LOG, 0,                         SUBMENU,             &log_menu},*/
		{ 0 } };

enum {
	IDLE = 0, BUTTON_UP, BUTTON_DOWN, BUTTON_ENTER, TIMEOUT
};

MenuItem * menu = &main_menu[0];
int menu_active = 0;

#define N_ELEM(array) (sizeof(array)/sizeof(array[0]))
#define menu_c    N_ELEM(menu)
#define roles_c   N_ELEM(roles)

void enter_menu(MenuItem *new_menu) {
	prev_menu[menu_depth] = menu;
	prev_menu_active[menu_depth] = menu_active;
	menu_depth++;
	menu = new_menu;
	menu_active = 0;
	lcd.clear();
}

void return_home(void) {
	if (menu == main_menu && menu_active == 0)
		return;
	lcd.clear();
	menu = main_menu;
	menu_active = 0;
}

void go_back(void) {
	menu_depth--;
	menu = prev_menu[menu_depth];
	menu_active = prev_menu_active[menu_depth];
	lcd.clear();
}

/****/

int message_ttl = 0;

void message(char *line1, char *line2,char* line3="",char* line4="") {

	lcd.clear();

	lcd.setPosition(0, 1);

	lcd.print(line1);

	lcd.setPosition(1, 0);
	if (line2)
		lcd.print(line2);
	lcd.setPosition(2,0);
	if(line3)
		lcd.print(line3);
	lcd.setPosition(3,0);
	if(line4)
		lcd.print(line4);

}

void print_time(int minutes_since_midnight);



int logno = 0;
void draw_log(void) {
	lcd.setPosition(0, 0);
	print_time(logno * LOG_INTERVAL);
	lcd.print(" ");
	lcd.print(datalog[logno].moisture);
	lcd.print("%  ");

	lcd.setPosition(0, 1);
	print_time((logno + 1) * LOG_INTERVAL);
	lcd.print(" ");
	if (logno + 1 >= 24 * 60 / LOG_INTERVAL)
		lcd.print(datalog[0].moisture);
	else
		lcd.print(datalog[logno + 1].moisture);
	lcd.print("%  ");

}
int offset = 0;

#define MIDNIGHT (60*24)
int get_seconds_since_midnight(void) {
	tmElements_t* tm;
	RTCread(tm);
	int hours = tm->Hour;
	int minutes=tm->Minute;
	int seconds=tm->Second;
	int totalseconds= hours*3600+ minutes*60+seconds;
	return totalseconds;
}
int get_minutes_since_midnight(void) {
		tmElements_t* tm;
		RTCread(tm);
		int hours = tm->Hour;
		int minutes=tm->Minute;
		int totalminutes= hours*60+ minutes;
		return totalminutes;
}



void reset_lights(void) {
	int i;
	for (i = 0; i < MAX_LIGHTS; i++) {
		lights_start[i] = 0;
		lights_duration[i] = 0;
	}
}



