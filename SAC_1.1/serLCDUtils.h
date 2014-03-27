/*
 * serLCDUtils.h
 *
 *  Created on: 16/03/2014
 *      Author: dcuevas
 */
#include "serLCD.h"
#include "RTCUtils.h"
#include "sac_sensors.h"
#include "Var.h"

#define LCD_PIN 11

#define MENU_SPEEDUP  3
#define MENU_TIMEOUT  120

/* distance between datalog entries in minutes */
#define LOG_INTERVAL 60

serLCD seriallcd(LCD_PIN);

//NUEVA LIBRERIA LCD CON FUNCIONES PREDEFINIDAS
//
#define LCD_SENDCOMMAND(command){seriallcd.print(0xFE); seriallcd.print(command); }
#define LCD_SPECIALCOMMAND(scommand){seriallcd.print(0x7C); seriallcd.print(scommand); }
//-------------------------------------------------------------------------------------------

#define MAX_LIGHTS 1

int lights_start[MAX_LIGHTS] = { 0, };
int lights_duration[MAX_LIGHTS] = { 0, };

void clearScreen() {
	//clears the screen, you will use this a lot!
	LCD_SENDCOMMAND(0x01);
2
}

//-------------------------------------------------------------------------------------------
void moveCursorRightOne() {
	//moves the cursor right one space
	LCD_SENDCOMMAND(0x14);
}
//-------------------------------------------------------------------------------------------
void moveCursorLeftOne() {
	//moves the cursor left one space
	LCD_SENDCOMMAND(0x10);
}
//-------------------------------------------------------------------------------------------
void scrollRight() {
	//same as moveCursorRightOne
	LCD_SENDCOMMAND(0x1C);
}
//-------------------------------------------------------------------------------------------
void scrollLeft() {
	//same as moveCursorLeftOne
	LCD_SENDCOMMAND(0x18);
}
//-------------------------------------------------------------------------------------------
void turnDisplayOff() {
	//this tunrs the display off, but leaves the backlight on.
	LCD_SENDCOMMAND(0x08);
}
//-------------------------------------------------------------------------------------------
void turnDisplayOn() {
	//this turns the display back ON
	LCD_SENDCOMMAND(0x0C);
}
//-------------------------------------------------------------------------------------------
void underlineCursorOn() {
	//turns the underline cursor on
	LCD_SENDCOMMAND(0x0E);
}
//-------------------------------------------------------------------------------------------
/*void underlineCursorOff()
 {
 //turns the underline cursor off
 LCD.print(0xFE); //command flag
 LCD.print(12); // 0x0C
 }*/
//-------------------------------------------------------------------------------------------
void boxCursorOn() {
	//this turns the box cursor on
	LCD_SENDCOMMAND(0x0D);
}
//-------------------------------------------------------------------------------------------
/*void boxCursorOff()
 {
 //this turns the box cursor off
 LCD.print(0xFE); //command flag
 LCD.print(12); // 0x0C
 }*/
//-------------------------------------------------------------------------------------------
void toggleSplash() {
	//this toggles the spalsh screenif off send this to turn onif on send this to turn off
	LCD_SPECIALCOMMAND(0x09);
}
//-------------------------------------------------------------------------------------------
int backlight(int brightness) // 128 = OFF, 157 = Fully ON, everything inbetween = varied brightnbess
		{
	//this function takes an int between 128-157 and turns the backlight on accordingly
	LCD_SPECIALCOMMAND(brightness);
}

//-------------------------------------------------------------------------------------------
void counter() {
	//this function prints a simple counter that counts to 10
	clearScreen();
	for (int i = 0; i <= 10; i++) {
		seriallcd.print("Counter = ");
		seriallcd.print(i, DEC);
		delay(500);
		clearScreen();
	}
}
//-------------------------------------------------------------------------------------------
void tempAndHumidity() {
	//this function shows how you could read the data from a temerature and humidity
	//sensro and then print that data to the SerLCD.

	//these could be varaibles instead of static numbers
	float tempF = 77.0;
	float tempC = 25.0;
	float humidity = 67.0;

	clearScreen();
	seriallcd.setCursor(0, 1);
	seriallcd.print(" Temp = ");
	seriallcd.print((long) tempF, DEC);
	seriallcd.print("F ");
	seriallcd.print((long) tempC, DEC);
	seriallcd.print("C");
	seriallcd.setCursor(1, 0);
	seriallcd.print(" Humidity = ");
	seriallcd.print((long) humidity, DEC);
	seriallcd.print("%");
	delay(2500);
}
//-------------------------------------------------------------------------------------------
void backlight() {
	//this function shows the different brightnesses to which the backlight can be set
	clearScreen();
	for (int i = 128; i < 158; i += 2) // 128-157 are the levels from off to full brightness
			{
		backlight(i);
		delay(100);
		seriallcd.print("Backlight = ");
		seriallcd.print(i, DEC);
		delay(500);
		clearScreen();
	}
}

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
	clearScreen();
}

void return_home(void) {
	if (menu == main_menu && menu_active == 0)
		return;
	clearScreen();
	menu = main_menu;
	menu_active = 0;
}

void go_back(void) {
	menu_depth--;
	menu = prev_menu[menu_depth];
	menu_active = prev_menu_active[menu_depth];
	clearScreen();
}

/****/

int message_ttl = 0;

void message(char *line1, char *line2) {

	clearScreen();
//  LCD.setCursor(0,0);
	seriallcd.setCursor(0, 1);
//  LCD.print(line1);
	seriallcd.print(line1);
//  LCD.setCursor(0,1);
	seriallcd.setCursor(1, 0);
	if (line2)
		seriallcd.print(line2);
	clearScreen();
	message_ttl = 3;
}

void print_time(int minutes_since_midnight);

void draw_status(int time, int moisture, int temperature, int humidity, char* time, char* date) {
	/*Printing Date & Time in first Row of status screen*/
	seriallcd.setCursor(0,1);
	seriallcd.print(getTime());
	seriallcd.setCursor(0,8);
	seriallcd.print(getDate());

	/*Printing Soil humidity Values in second row of status screen*/
	seriallcd.setCursor(1,0);
	seriallcd.print(translate(S_S));
	seriallcd.setCursor(1,4);
	seriallcd.print(moisture_target);
	seriallcd.setCursor(1,6);
	seriallcd.print("% ");
	seriallcd.setCursor(1, 7);
	//TODO  Falta imprimir el minimo tomado por entrada de usuario
	seriallcd.print(translate(MIN));
	seriallcd.setCursor(1,11);
	seriallcd.print(soil_moisture_MIN);
	seriallcd.setCursor(1,12);
	seriallcd.Print("[");
	seriallcd.setCursor(1,14);
	seriallcd.print(moisture);
	seriallcd.setCursor(1,16);
	seriallcd.Print("]");
	/*Third Row will be for printing data related to watering Cycles*/
	seriallcd.setCursor(2,0);
	seriallcd.print(translate(S_LENGTH_SEC));
	seriallcd.setCursor(2,13);
	seriallcd.print(pump_cycle_length);
	seriallcd.setCursor(2,15);
	seriallcd.print(translate(S_ON));
	/*Fourth row is going to print info related to Soil temperature*/
	seriallcd.setCursor(3,0);
	seriallcd.Print(translate(ST_MAX));
	seriallcd.setCursor(3,6);
	seriallcd.print(temperature);
	seriallcd.setCursor(3,8);
	seriallcd.Print(translate(MIN));

	seriallcd.print("C");

	/*LAST ROW WILL PRINT WATER CONSUMPTION IN CUBIC METERS*/
	seriallcd.setCursor(4,0);
	seriallcd.print(CONSUMPTION);


	seriallcd.print("% HR");
	seriallcd.setCursor(0, 1);
	seriallcd.print(translate(S_S));

	{
		Relay *light = find_relay(LIGHT);
		if (light) {
			seriallcd.setCursor(6, 1);
			if (light->state == RELAY_ON)
				seriallcd.print(translate(S_L));
		}
	}

	{
		Relay *heating = find_relay(HEATING);
		Relay *cooling = find_relay(COOLING);
		if (heating) {
			seriallcd.setCursor(8, 1);
			if (heating->state == RELAY_ON)
				seriallcd.print("+");
			else {
				if (cooling && cooling->state == RELAY_ON)
					seriallcd.print("-");
				else
					seriallcd.print(" ");
			}
		}
	}

	{
		Relay *ventilation = find_relay(VENTILATION);
		Relay *humidifier = find_relay(HUMIDIFIER);
		if (ventilation && ventilation->state == RELAY_ON) {
			seriallcd.setCursor(3, 1);
			seriallcd.print(translate(S_V));
		} else if (humidifier && humidifier->state == RELAY_ON) {
			seriallcd.setCursor(3, 1);
			seriallcd.print(translate(S_H));
		}
	}

	{
		Relay *water = find_relay(IRRIGATION);
		if (water) {
			seriallcd.setCursor(0, 1);
			if (cached_water_level)
				seriallcd.print("     "); /* LCD.print(translate(S_WA)); */
			else
				seriallcd.print(translate(S_WA));

			seriallcd.setCursor(0, 1);
			switch (water->state) {
			case RELAY_ON:
				seriallcd.print(translate(S_PU));
				break;
			case RELAY_OFF:
				seriallcd.print("  ");
				break;
			case RELAY_WAITING:
				seriallcd.print(translate(S_PU));
				break;
			}
		}
	}

	seriallcd.setCursor(0, 0);
	{
		print_time(time);
	}
}

int logno = 0;
void draw_log(void) {
	seriallcd.setCursor(0, 0);
	print_time(logno * LOG_INTERVAL);
	seriallcd.print(" ");
	seriallcd.print(datalog[logno].moisture);
	seriallcd.print("%  ");

	seriallcd.setCursor(0, 1);
	print_time((logno + 1) * LOG_INTERVAL);
	seriallcd.print(" ");
	if (logno + 1 >= 24 * 60 / LOG_INTERVAL)
		seriallcd.print(datalog[0].moisture);
	else
		seriallcd.print(datalog[logno + 1].moisture);
	seriallcd.print("%  ");

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
		clearScreen();
	}
	seriallcd.setCursor(0, 0);

	switch (mi->type) {
	case STATUS:
		draw_status(get_minutes_since_midnight(), moisture_read(),
				cached_temperature, cached_humidity, char* time, char* date);
		clearScreen();
		return;

	case LOG:
		draw_log();
		clearScreen();
		return;

	default:

		seriallcd.print(translate(mi->label));
		seriallcd.setCursor(0, 1);
		if (mi->label2)
			seriallcd.print(translate(mi->label2));

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
			seriallcd.setCursor(0, 1);
			tmElements_t tm;
			RTCread(&tm);
			seriallcd.print((float)getSeconds(tm));
		}
			break;
		case TEXT:
			seriallcd.print((char *) mi->data);
			break;
		case NUMBER:
			if (is_editing) {
				int ones, tens;
				tens = (*(float*) (mi->data));
				ones = tens % 10;
				tens -= ones;
				tens /= 10;

				if (is_editing == 1) {
					seriallcd.print(tens);
					seriallcd.print(ones);
				} else if (is_editing == 2) {
					seriallcd.print(tens);
					seriallcd.print(ones);
				} else
					seriallcd.print((*(float*) (mi->data)));
			} else
				seriallcd.print((*(float*) (mi->data)));
			break;

		case SOIL_CALIBRATE:
			if (is_editing)
				seriallcd.print((*(float*) (mi->data)));
			else
				seriallcd.print((*(float*) (mi->data)));
			/*seriallcd.print("s ");*/
			seriallcd.print(" -ENTER- ");
			seriallcd.print(cached_moisture);
			seriallcd.print("   ");
			/*seriallcd.print("v ");*/
			/*seriallcd.print(moisture_read());*/

			break;

		case ROLE: {
			int role = (*((int*) (mi->data)));
			seriallcd.print("[");
			if (is_editing)
				seriallcd.print(translate(roles[role]));
			else
				seriallcd.print(translate(roles[role]));
			seriallcd.print("]");
		}
			break;

		case LANGUAGE: {
			seriallcd.print("[");
			if (is_editing)
				seriallcd.print(translate(S_ENGLISH));
			else
				seriallcd.print(translate(S_ENGLISH));
			seriallcd.print("]");
		}
			break;

		case ONOFF:
			if (*((int*) (mi->data)))
				seriallcd.print(translate(S_ENABLED));
			else
				seriallcd.print(translate(S_DISABLED));
			break;

		default:
			break;
		}

	clearScreen();
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
				clearScreen();
				break;
			case BUTTON_DOWN:
				(*val) += 1;
				if ((*val) >= roles_c)
					*val = 0;
				clearScreen();
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
				clearScreen();
				break;
			case BUTTON_DOWN:
				logno++;
				if (logno >= 24 * 60 / LOG_INTERVAL)
					logno = 0;
				clearScreen();
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
				clearScreen();
				break;
			case BUTTON_DOWN:
				(*val) += 1;
				if ((*val) >= MAX_LANGUAGE)
					*val = 0;
				clearScreen();
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
		seriallcd.print("");
		menu_enter();
		break;
	case BUTTON_UP:
		menu_active--;
		if (menu_active < 0)
			while (menu[menu_active + 1].label)
				menu_active++;
		seriallcd.print("");
		clearScreen();
		break;
	case BUTTON_DOWN:
		menu_active++;
		if (menu[menu_active].label == 0)
			menu_active = 0;
		seriallcd.print("");
		clearScreen();
		break;
	case IDLE:
		idle_count++;
		if (idle_count * 1.0 < MENU_TIMEOUT)
			break;

		if (is_editing)
			break; /* do not jump back to start when editing */
		/* fallthrough */
	case TIMEOUT:
		seriallcd.print("OFF");
		return_home();
		break;
	default:
		break;
	}
}

