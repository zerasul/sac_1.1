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

#define LCD_COLS 20
#define LCD_ROWS 4

/* distance between datalog entries in minutes */
#define LOG_INTERVAL 60

SoftwareSerial SSerial(0,LCD_PIN);

SerLCD Lcd(SSerial,LCD_COLS,LCD_ROWS);




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
	Lcd.clear();
}

void return_home(void) {
	if (menu == main_menu && menu_active == 0)
		return;
	Lcd.clear();
	menu = main_menu;
	menu_active = 0;
}

void go_back(void) {
	menu_depth--;
	menu = prev_menu[menu_depth];
	menu_active = prev_menu_active[menu_depth];
	Lcd.clear();
}

/****/

int message_ttl = 0;

void message(char *line1, char *line2, char* line3, char* line4) {

	Lcd.clear();

	Lcd.setPosition(0, 0);

	Lcd.print(line1);

	Lcd.setPosition(1, 0);
	if (line2)
		Lcd.print(line2);
	Lcd.setPosition(2,0);
	if(line3)
		Lcd.print(line3);
	Lcd.setPosition(2,0);
		if(line4)
			Lcd.print(line4);

}

void print_time(int minutes_since_midnight);

void draw_status(int time, int moisture, int temperature, int humidity, char* time, char* date) {
	/*Printing Date & Time in first Row of status screen*/
	Lcd.setPosition(0,1);
	Lcd.print(getTime());
	Lcd.setPosition(0,8);
	Lcd.print(getDate());

	/*Printing Soil humidity Values in second row of status screen*/
	Lcd.setPosition(1,0);
	Lcd.print(translate(S_S));
	Lcd.setPosition(1,4);
	Lcd.print(moisture_target);//TODO:Poner por parametro
	Lcd.setPosition(1,6);
	Lcd.print("% ");
	Lcd.setPosition(1, 7);
	//TODO  Falta imprimir el minimo tomado por entrada de usuario
	Lcd.print(translate(MIN));
	Lcd.setPosition(1,11);
	Lcd.print(soil_moisture_MIN);//TODO: Por parametro
	Lcd.setPosition(1,12);
	Lcd.print("[");
	Lcd.setPosition(1,14);
	Lcd.print(moisture);
	Lcd.setPosition(1,16);
	Lcd.print("]");
	/*Third Row will be for printing data related to watering Cycles*/
	Lcd.setPosition(2,0);
	Lcd.print(translate(S_LENGTH_SEC));
	Lcd.setPosition(2,13);
	Lcd.print(pump_cycle_length);//TODO: Poner longitud ciclo por intervalos
	Lcd.setPosition(2,15);
	Lcd.print(S_ON);
	Lcd.setPosition(2,18);
	//Lcd.print(cycle_percent); TODO: tomar por parametro
	/*Fourth row is going to print info related to Soil temperature*/

	Lcd.setPosition(3,0);
	Lcd.print(ST_MAX);
	Lcd.setPosition(3,6);
	//Lcd.print(temperaturemax); TODO: Tomar por parametro temp max.

	Lcd.setPosition(3,8);
	Lcd.print(MIN);
	//Lcd.print(temperaturemin); TODO: Tomar por parametro temp min.
	Lcd.setPosition(3,15);
	Lcd.print(temperature);
	Lcd.print("C");

	/*LAST ROW WILL PRINT WATER CONSUMPTION IN CUBIC METERS*/
	Lcd.setPosition(4,0);
	Lcd.print(translate(CONSUMPTION));

    Lcd.print(getWaterFlowRate());
	Lcd.print("M3");
	{
		Relay *light = find_relay(LIGHT);
		if (light) {
			Lcd.setPosition(6, 1);
			if (light->state == RELAY_ON)
				Lcd.print(translate(S_L));
		}
	}

	{
		Relay *heating = find_relay(HEATING);
		Relay *cooling = find_relay(COOLING);
		if (heating) {
			Lcd.setPosition(8, 1);
			if (heating->state == RELAY_ON)
				Lcd.print("+");
			else {
				if (cooling && cooling->state == RELAY_ON)
					Lcd.print("-");
				else
					Lcd.print(" ");
			}
		}
	}

	{
		Relay *ventilation = find_relay(VENTILATION);
		Relay *humidifier = find_relay(HUMIDIFIER);
		if (ventilation && ventilation->state == RELAY_ON) {
			Lcd.setPosition(3, 1);
			Lcd.print(translate(S_V));
		} else if (humidifier && humidifier->state == RELAY_ON) {
			Lcd.setPosition(3, 1);
			Lcd.print(translate(S_H));
		}
	}

	{
		Relay *water = find_relay(IRRIGATION);
		if (water) {
			Lcd.setPosition(0, 1);
			if (cached_water_level)
				Lcd.print("     "); /* LCD.print(translate(S_WA)); */
			else
				Lcd.print(translate(S_WA));

			Lcd.setPosition(0, 1);
			switch (water->state) {
			case RELAY_ON:
				Lcd.print(translate(S_PU));
				break;
			case RELAY_OFF:
				Lcd.print("  ");
				break;
			case RELAY_WAITING:
				Lcd.print(translate(S_PU));
				break;
			}
		}
	}

	Lcd.setPosition(0, 0);
	{
		print_time(time);
	}
}

int logno = 0;
void draw_log(void) {
	Lcd.setPosition(0, 0);
	print_time(logno * LOG_INTERVAL);
	Lcd.print(" ");
	Lcd.print(datalog[logno].moisture);
	Lcd.print("%  ");

	Lcd.setPosition(0, 1);
	print_time((logno + 1) * LOG_INTERVAL);
	Lcd.print(" ");
	if (logno + 1 >= 24 * 60 / LOG_INTERVAL)
		Lcd.print(datalog[0].moisture);
	else
		Lcd.print(datalog[logno + 1].moisture);
	Lcd.print("%  ");

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

void draw_ui(float cached_temperature, float cached_humidity) {
	MenuItem *mi = &menu[menu_active];

	if (message_ttl) {
		message_ttl--;
		if (message_ttl > 0)
			return;
		Lcd.clear();
	}
	Lcd.setPosition(0, 0);

	switch (mi->type) {
	case STATUS:
		draw_status(get_minutes_since_midnight(), moisture_read(),
				cached_temperature, cached_humidity, char* time, char* date);
		Lcd.clear();
		return;

	case LOG:
		draw_log();
		Lcd.clear();
		return;

	default:

		Lcd.print(translate(mi->label));
		Lcd.setPosition(0, 1);
		if (mi->label2)
			Lcd.print(translate(mi->label2));

		break;
	}

	if (mi->data || mi->type == UPTIME)
		switch (mi->type) {
		case TIME:
			/* special casing for showing the time when not
			 * editing it
			 */
			if (!is_editing)
				minutes = get_minutes_since_midnight();

			{
				int *time = ((int*) (mi->data));
				int foo = *time;
				print_time(foo);
			}
			break;
		case UPTIME: {
			Lcd.setPosition(0, 1);
			tmElements_t tm;
			RTCread(&tm);
			Lcd.print((float)getSeconds(tm));
		}
			break;
		case TEXT:
			Lcd.print((char *) mi->data);
			break;
		case NUMBER:
			if (is_editing) {
				int ones, tens;
				tens = (*(float*) (mi->data));
				ones = tens % 10;
				tens -= ones;
				tens /= 10;

				if (is_editing == 1) {
					Lcd.print(tens);
					Lcd.print(ones);
				} else if (is_editing == 2) {
					Lcd.print(tens);
					Lcd.print(ones);
				} else
					Lcd.print((*(float*) (mi->data)));
			} else
				Lcd.print((*(float*) (mi->data)));
			break;

		case SOIL_CALIBRATE:
			if (is_editing)
				Lcd.print((*(float*) (mi->data)));
			else
				Lcd.print((*(float*) (mi->data)));
			/*Lcd.print("s ");*/
			Lcd.print(" -ENTER- ");
			Lcd.print(cached_moisture);
			Lcd.print("   ");
			/*Lcd.print("v ");*/
			/*Lcd.print(moisture_read());*/

			break;

		case ROLE: {
			int role = (*((int*) (mi->data)));
			Lcd.print("[");
			if (is_editing)
				Lcd.print(translate(roles[role]));
			else
				Lcd.print(translate(roles[role]));
			Lcd.print("]");
		}
			break;

		case LANGUAGE: {
			Lcd.print("[");
			if (is_editing)
				Lcd.print(translate(S_ENGLISH));
			else
				Lcd.print(translate(S_ENGLISH));
			Lcd.print("]");
		}
			break;

		case ONOFF:
			if (*((int*) (mi->data)))
				Lcd.print(translate(S_ENABLED));
			else
				Lcd.print(translate(S_DISABLED));
			break;

		default:
			break;
		}

	Lcd.clear();
}

void reset_lights(void) {
	int i;
	for (i = 0; i < MAX_LIGHTS; i++) {
		lights_start[i] = 0;
		lights_duration[i] = 0;
	}
}

void menu_enter(void) {
	MenuItem *mi = &menu[menu_active];

	if (mi->type == STATUS)
		return;
	if (mi->type == SUBMENU) {
		enter_menu((MenuItem*) mi->data);
		return;
	}
	if (mi->type == BACK) {
		go_back();
		return;
	}

	if (mi->type == LIGHTS_RESET) {
		reset_lights();
		message("Reset lighting", "schedule.");
		end_editing();
	} else if (mi->type == STORE_SETTINGS) {
		store_settings(config);
		message("Settings stored.", "");
	} else if (mi->type == LOAD_SETTINGS) {
		load_settings(config);
	} else if (mi->type == RESET_SETTINGS) {
		reset_settings(config);
		end_editing();
		message("Factory defaults", "");
	} else if (mi->type == ONOFF) {
		int *val = (int*) mi->data;
		*val = !*val;
		end_editing();
	} else {
		is_editing = 1;
	}
}

void editing_handle_events(int event) {
	MenuItem *mi = &menu[menu_active];

	if (!(mi->type == NUMBER || mi->type == ROLE || mi->type == LANGUAGE
			|| mi->type == TIME || mi->type == LOG || mi->type == SOIL_CALIBRATE)) {
		end_editing();
		return;
	}

	if ((mi->type == NUMBER || mi->type == TIME) && event == BUTTON_ENTER) {
		is_editing++;
		if (is_editing >= 3) {
			if (mi->data == &minutes) {
				/* store back edited tim */
				offset = (minutes - (get_minutes_since_midnight() - offset));
			}
			end_editing();
			return;
		}
	} else if (event == BUTTON_ENTER) {
		end_editing();
	}

	if (mi->data || mi->type == LOG)
		switch (mi->type) {
		case ROLE: {
			int *val = (int*) mi->data;
			switch (event) {
			case BUTTON_UP:
				(*val) -= 1;
				if (*val < 0)
					*val = roles_c - 1;
				Lcd.clear();
				break;
			case BUTTON_DOWN:
				(*val) += 1;
				if ((*val) >= roles_c)
					*val = 0;
				Lcd.clear();
				break;
			case IDLE:
			case TIMEOUT:
			case BUTTON_ENTER:
				break;
			}
		}
			break;

		case LOG: {
			switch (event) {
			case BUTTON_UP:
				logno--;
				if (logno < 0)
					logno = (24 * 60 / LOG_INTERVAL) - 1;
				Lcd.clear();
				break;
			case BUTTON_DOWN:
				logno++;
				if (logno >= 24 * 60 / LOG_INTERVAL)
					logno = 0;
				Lcd.clear();
				break;
			case IDLE:
			case TIMEOUT:
			case BUTTON_ENTER:
				break;
			}
		}
			break;

		case LANGUAGE: {
			int *val = (int*) mi->data;
			switch (event) {
			case BUTTON_UP:
				(*val) -= 1;
				if (*val < 0)
					*val = roles_c - 1;
				Lcd.clear();
				break;
			case BUTTON_DOWN:
				(*val) += 1;
				if ((*val) >= MAX_LANGUAGE)
					*val = 0;
				Lcd.clear();
				break;
			case IDLE:
			case TIMEOUT:
			case BUTTON_ENTER:
				break;
			}
		}
			break;

		case NUMBER: {
			float *val = (float*) mi->data;
			switch (event) {
			case BUTTON_UP:
				if (is_editing == 1)
					(*val) += 10;
				else
					(*val) += 1;
				if (*val > 100)
					*val = 100;
				break;
			case BUTTON_DOWN:
				if (is_editing == 1)
					(*val) -= 10;
				else
					(*val) -= 1;

				if (*val < 0)
					*val = 0;
				break;
			case IDLE:
			case TIMEOUT:
			case BUTTON_ENTER:
				break;
			}
			break;
		}

		case TIME: {
			int *val = (int*) mi->data;
			switch (event) {
			case BUTTON_UP:
				if (is_editing == 1)
					(*val) += 60;
				else
					(*val) += 1;

				if (*val > MIDNIGHT)
					*val = *val - MIDNIGHT;
				break;
			case BUTTON_DOWN:
				if (is_editing == 1)
					(*val) -= 60;
				else
					(*val) -= 1;

				if (*val < 0)
					*val = MIDNIGHT - *val;
				break;
			case IDLE:
			case TIMEOUT:
			case BUTTON_ENTER:
				break;
			}
			break;
		}

		case SOIL_CALIBRATE:
			moisture_calib = cached_moisture;
			is_editing = 0;
		case IDLE:
			break;
		default:
			break;
		}
}
void handle_events(void) {
	int event = get_event();
	static int idle_count = 0;

	if (is_editing) {
		editing_handle_events(event);
		return;
	}

	if (event != IDLE)
		idle_count = 0;

	switch (event) {
	case BUTTON_ENTER:
		Lcd.print("");
		menu_enter();
		break;
	case BUTTON_UP:
		menu_active--;
		if (menu_active < 0)
			while (menu[menu_active + 1].label)
				menu_active++;
		Lcd.print("");
		Lcd.clear();
		break;
	case BUTTON_DOWN:
		menu_active++;
		if (menu[menu_active].label == 0)
			menu_active = 0;
		Lcd.print("");
		Lcd.clear();
		break;
	case IDLE:
		idle_count++;
		if (idle_count * 1.0 < MENU_TIMEOUT)
			break;

		if (is_editing)
			break; /* do not jump back to start when editing */
		/* fallthrough */
	case TIMEOUT:
		Lcd.print("OFF");
		return_home();
		break;
	default:
		break;
	}
}

