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
