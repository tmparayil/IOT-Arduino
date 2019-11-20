/*
  Hello World.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

  Grove - Serial LCD RGB Backlight demo.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Wire.h>
#include "rgb_lcd.h"
#include <math.h>
#include <Servo.h>
#include <Grove_LED_Bar.h>
int a,b;
float temperature;
int B=3975;                  //B value of the thermistor
float resistance;
rgb_lcd lcd;



const int colorR = 100;
const int colorG = 50;
const int colorB = 120;

Servo myservo;

void setup() 
{
    // set up the LCD's number of columns and rows:
    lcd.begin(1, 2);
    
    //lcd.setRGB(colorR, colorG, colorB);
    myservo.attach(3);
    // Print a message to the LCD.
    lcd.print("hello, world!");
    randomSeed(analogRead(1));
    delay(1000);

  
  
    
}

void loop() 
{
    a=analogRead(0);
    b = analogRead(3);
    resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
    temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    
    int red = random(0,255);
    int blu = random(0,255);
    int gre = random(0,255);
    lcd.setRGB(red, blu, gre);

  int value = analogRead(A0);
  value = map(b, 22, 764, 0, 180);

  
  delay(100);

  lcd.print(b);
  
  myservo.write(value);
    
    delay(100);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
