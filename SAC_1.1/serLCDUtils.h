/*
 * serLCDUtils.h
 *
 *  Created on: 16/03/2014
 *      Author: dcuevas
 */
#include "serLCD.h"

#define LCD_PIN 11

serLCD seriallcd(LCD_PIN);

//NUEVA LIBRERIA LCD CON FUNCIONES PREDEFINIDAS
//
#define LCD_SENDCOMMAND(command){seriallcd.print(0xFE); seriallcd.print(command); }
#define LCD_SPECIALCOMMAND(scommand){seriallcd.print(0x7C); seriallcd.print(scommand); }
//-------------------------------------------------------------------------------------------
void clearScreen()
{
  //clears the screen, you will use this a lot!
  LCD_SENDCOMMAND(0x01);

}

//-------------------------------------------------------------------------------------------
void moveCursorRightOne()
{
  //moves the cursor right one space
LCD_SENDCOMMAND(0x14);
}
//-------------------------------------------------------------------------------------------
void moveCursorLeftOne()
{
  //moves the cursor left one space
LCD_SENDCOMMAND(0x10);
}
//-------------------------------------------------------------------------------------------
void scrollRight()
{
  //same as moveCursorRightOne
LCD_SENDCOMMAND(0x1C);
}
//-------------------------------------------------------------------------------------------
void scrollLeft()
{
  //same as moveCursorLeftOne
LCD_SENDCOMMAND(0x18);
}
//-------------------------------------------------------------------------------------------
void turnDisplayOff()
{
  //this tunrs the display off, but leaves the backlight on.
LCD_SENDCOMMAND(0x08);
}
//-------------------------------------------------------------------------------------------
void turnDisplayOn()
{
  //this turns the display back ON
LCD_SENDCOMMAND(0x0C);
}
//-------------------------------------------------------------------------------------------
void underlineCursorOn()
{
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
void boxCursorOn()
{
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
void toggleSplash()
{
  //this toggles the spalsh screenif off send this to turn onif on send this to turn off
LCD_SPECIALCOMMAND(0x09);
}
//-------------------------------------------------------------------------------------------
int backlight(int brightness)// 128 = OFF, 157 = Fully ON, everything inbetween = varied brightnbess
{
  //this function takes an int between 128-157 and turns the backlight on accordingly
LCD_SPECIALCOMMAND(brightness);
}

//-------------------------------------------------------------------------------------------
void counter()
{
  //this function prints a simple counter that counts to 10
  clearScreen();
  for(int i = 0; i <= 10; i++)
  {
    seriallcd.print("Counter = ");
    seriallcd.print(i, DEC);
    delay(500);
    clearScreen();
  }
}
//-------------------------------------------------------------------------------------------
void tempAndHumidity()
{
  //this function shows how you could read the data from a temerature and humidity
  //sensro and then print that data to the SerLCD.

  //these could be varaibles instead of static numbers
  float tempF = 77.0;
  float tempC = 25.0;
  float humidity = 67.0;

  clearScreen();
  seriallcd.setCursor(0,1);
  seriallcd.print(" Temp = ");
  seriallcd.print((long)tempF, DEC);
  seriallcd.print("F ");
  seriallcd.print((long)tempC, DEC);
  seriallcd.print("C");
  seriallcd.setCursor(1,0);
  seriallcd.print(" Humidity = ");
  seriallcd.print((long)humidity, DEC);
  seriallcd.print("%");
  delay(2500);
}
//-------------------------------------------------------------------------------------------
void backlight()
{
  //this function shows the different brightnesses to which the backlight can be set
  clearScreen();
  for(int i = 128; i < 158; i+=2)// 128-157 are the levels from off to full brightness
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

MenuItem relay_menu[] = {
  {S_RELAY1, S_EMPTY, ROLE, &relay[0].role},
  {S_RELAY2, S_EMPTY, ROLE, &relay[1].role},
  {S_RELAY3, S_EMPTY, ROLE, &relay[2].role},
  {S_RETURN_TO, S_MAIN_MENU,       BACK, 0},
  {0}
};

int minutes = 0;

#define MAX_MENU_DEPTH 4

MenuItem *prev_menu[MAX_MENU_DEPTH];
int       prev_menu_active[MAX_MENU_DEPTH];
int       menu_depth = 0;

MenuItem log_menu[] =
{
  {S_LOG, 0, LOG, },
  {S_RETURN_TO, S_MAIN_MENU,       BACK, 0},
  {0}
};

MenuItem main_menu[] = {
  {S_A, 0,                           STATUS,              (void*)0},
  {S_RELAYS, S_CONFIG,               SUBMENU,             &relay_menu},
  {S_SOIL_MOISTURE, 0,               NUMBER,              &moisture_target},
  {S_IRRIGATION_CYCLE, S_LENGTH_SEC, NUMBER,              &pump_cycle_length},
  {S_CALIBRATE_MOIST, 0,             SOIL_CALIBRATE,      &moisture_calib},
  {S_AIR_HUMIDITY, 0,                NUMBER,              &humidity_target},
  {S_AIR_TEMPERATURE, 0,             NUMBER,              &temperature_target},
  {S_LIGHT,  S_START,                TIME,                &lights_start[0]},
  {S_LIGHT,  S_DURATION,             TIME,                &lights_duration[0]},
  {S_TIME, 0,                        TIME,                &minutes},
  {S_LANGUAGE, 0,                    LANGUAGE,            &active_language},
/*{S_RESET, S_CONFIG,                RESET_SETTINGS,      0},*/
/*{S_LOG, 0,                         SUBMENU,             &log_menu},*/
  {0}
};

enum {
  IDLE = 0,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_ENTER,
  TIMEOUT
};

MenuItem * menu = &main_menu[0];
int menu_active = 0;

#define N_ELEM(array) (sizeof(array)/sizeof(array[0]))
#define menu_c    N_ELEM(menu)
#define roles_c   N_ELEM(roles)

void enter_menu (MenuItem *new_menu)
{
  prev_menu[menu_depth] = menu;
  prev_menu_active[menu_depth] = menu_active;
  menu_depth++;
  menu = new_menu;
  menu_active = 0;
  clearScreen();
}

void return_home (void)
{
  if (menu == main_menu && menu_active == 0)
    return;
  clearScreen();
  menu = main_menu;
  menu_active = 0;
}

void go_back (void)
{
  menu_depth--;
  menu = prev_menu[menu_depth];
  menu_active = prev_menu_active[menu_depth];
  clearScreen();
}

/****/

int message_ttl = 0;

void message(char *line1, char *line2)
{

clearScreen();
//  LCD.setCursor(0,0);
seriallcd.setCursor(0,1);
//  LCD.print(line1);
seriallcd.print(line1);
//  LCD.setCursor(0,1);
seriallcd.setCursor(1,0);
  if (line2)
    seriallcd.print(line2);
  clearScreen();
  message_ttl = 3;
}

void draw_status (int time,
                  int moisture,
                  int temperature,
                  int humidity)
{
  seriallcd.setCursor(6,0);
/*LCD.print(translate(S_A));*/
  seriallcd.print(temperature);
  seriallcd.print("C");

//  LCD.setCursor(0, 1);
  seriallcd.print(humidity);
  seriallcd.print("% HR");
  seriallcd.setCursor(0, 1);
  seriallcd.print(translate(S_S));
  seriallcd.print(moisture_target);
  seriallcd.print("% ");
  seriallcd.setCursor(8,1);
  seriallcd.print(translate(MIN));
  seriallcd.setCursor(16,1);
  seriallcd.print("[");
  seriallcd.setCursor(18,1);
  seriallcd.print(moisture);
  seriallcd.setCursor(19,1);
  seriallcd.print("]");




  {
    Relay *light = find_relay (LIGHT);
    if (light)
    {
      seriallcd.setCursor(6, 1);
      if (light->state == RELAY_ON)
        seriallcd.print(translate(S_L));
    }
  }

  {
    Relay *heating = find_relay (HEATING);
    Relay *cooling = find_relay (COOLING);
    if (heating)
    {
      seriallcd.setCursor(8, 1);
      if (heating->state == RELAY_ON)
        seriallcd.print("+");
      else
        {
          if (cooling && cooling->state == RELAY_ON)
            seriallcd.print("-");
          else
            seriallcd.print(" ");
        }
    }
  }

  {
    Relay *ventilation = find_relay (VENTILATION);
    Relay *humidifier = find_relay (HUMIDIFIER);
    if (ventilation && ventilation->state == RELAY_ON)
      {
        seriallcd.setCursor(3, 1);
        seriallcd.print(translate(S_V));
      }
    else if (humidifier && humidifier->state == RELAY_ON)
      {
        seriallcd.setCursor(3, 1);
        seriallcd.print(translate(S_H));
      }
  }



  {
    Relay *water = find_relay (IRRIGATION);
    if (water)
    {
      seriallcd.setCursor(0,1);
      if (cached_water_level)
        seriallcd.print("     "); /* LCD.print(translate(S_WA)); */
      else
        seriallcd.print(translate(S_WA));

      seriallcd.setCursor(0,1);
      switch (water->state)
        {
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

  seriallcd.setCursor(0,0);
  {
    print_time (time);
  }
}

int logno = 0;
void draw_log (void)
{
  seriallcd.setCursor(0,0);
  print_time(logno*LOG_INTERVAL);
  seriallcd.print(" ");
  seriallcd.print(datalog[logno].moisture);
  seriallcd.print("%  ");

  seriallcd.setCursor(0,1);
  print_time((logno+1)*LOG_INTERVAL);
  seriallcd.print(" ");
  if (logno+1 >= 24 * 60 / LOG_INTERVAL)
  seriallcd.print(datalog[0].moisture);
  else
  seriallcd.print(datalog[logno+1].moisture);
  seriallcd.print("%  ");

}
