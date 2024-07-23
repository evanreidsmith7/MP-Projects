/*
ee3420lab5.ino
date: 10/31/22
Written by: Evan smith
this program is designed to use the mega adc board to read analogue data
such as temperature, voltage ratio from a potentiometer to control a servo,
detects light and turns on led if its dark, reads position of joystick and calulate
magnitude and direction of vector made by stick
*/
#include <ASCII.h>
//libray for mega aux board
#include <ee3420_mega_aux_board.h>
//commonly useful system libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
//libray for mega adc board
#include "ee3420_mega_adc_board.h"


void setup() 
{
  // put your setup code here, to run once:
    #ifdef EE3420_MEGA_AUX_H
  ee3420_mega_aux_setup();
  #endif
  #ifdef EE3420_MEGA_ADC_H
    ee3420_mega_adc_setup();
  #endif
  
  Serial.begin(115200, SERIAL_8N1);
  lcd.begin(16,2);
 


}
char key_input[3]={0,0,0};
bool key = true;
int raw_x, raw_y;
int raw_sample;
bool motor = false;
void loop()
{
  // user input
  while (key)
  {
    char keypad_current_key = keypad.getKey();
    if (keypad_current_key != NO_KEY)
    {
      
      char input = keypad_current_key;
      if (input = 'A')
        motor = true;
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
    }
    if (Serial.available())
    {
      char input = Serial.read();
      if (input = 'A')
        motor = true;
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
      while (Serial.available()){Serial.read();}
    }
    if((key_input[0]=='*')&&(key_input[1]=='*')&&(key_input[2]=='*')) 
    {
      key = false;
    }
    //runs through sensor functions
    lcd.clear();
    joystick();
    temperature();
    delay(1000);
    lcd.clear();
    photo();
    delay(1000);
    lcd.clear();
    
    if(digitalRead(LED_YELLOW) && digitalRead(LED_RED) && digitalRead(LED_WHITE))
    {
      digitalWrite(LED_GREEN,1);
    }
    else
      digitalWrite(LED_GREEN, 0);
    //servo motor mode
    while (motor)
    {
      Serial.println("key to stop servo");
      if (Serial.available())
        motor = false;
      serv();
      
    }
    Serial.println();
    Serial.println();
  }
}
//uses potentiometer to calculate pulse width for servo
void serv()
{
  raw_sample=analogRead(POTENTIOMETER);
  float v = calculate_volts(raw_sample);
  float pw = ((v/5)*1000)+ 1000;
  Serial.println(pw);
  servo0.writeMicroseconds((int)pw);
}
//reads light and determines if led needs to be on
void photo()
{
  raw_sample=analogRead(PHOTOCELL);
  float r = calculate_photocell_resistance(raw_sample;
  lcd.setCursor(0,0);
  if (r >= 10000)
  {
    digitalWrite(LED_WHITE, 1);
    Serial.println("dark");
    lcd.print("dark ");
  }
  else
  {
    digitalWrite(LED_WHITE, 0);
  }
  Serial.print("Photo Resistance: ");
  Serial.print(r);
  Serial.println("ohms");
  lcd.print("Photo Resistance: ");
  lcd.setCursor(0,1);
  lcd.print(r);
  lcd.print("ohms");

}
//sensor for temperature
void temperature()
{
  raw_sample=analogRead(ADC_TEMPERATURE);
  Serial.print(calculate_MCP9701_fahrenheit(raw_sample));
  double temp = calculate_MCP9701_fahrenheit(raw_sample);
  Serial.println(" degrees F");
  lcd.setCursor(0,1);
  lcd.print(calculate_MCP9701_fahrenheit(raw_sample));
  lcd.print(" F");
  
  if (temp >= 77)
  {
    digitalWrite(LED_RED, 1);
    Serial.println("HOT");
    lcd.print(" HOT");
  }
  else
  {
    digitalWrite(LED_RED, 0);
  }
  
}
//reads position of joystick calculates magnitude and angle of direction
void joystick()
{
  raw_x=analogRead(JOYSTICK_X);
  raw_y=analogRead(JOYSTICK_Y);
  double x = raw_x - 504.0;
  double y = 509.0 - raw_y;
  

  lcd.setCursor(0,0);
  Serial.print("magnitude: ");
  Serial.println(mag(x, y));
  lcd.print(mag(x,y));
  lcd.print(" ");
  Serial.print("theta: ");
  Serial.println(theta(x,y));
  lcd.print(theta(x,y));    
}
//function for calculating magnitude taking x and y coords
double mag(double x, double y)
{
  double magnitude = sqrt(square(x) + square(y));
  if (magnitude > 0.25)
  {
    digitalWrite(LED_YELLOW, 1);
  }
  else
  {
    digitalWrite(LED_YELLOW, 0);
  }
  return magnitude;
}
//function for calculating angle taking x and y coords
double theta(double x, double y)
{
  double pi = 3.14;
  double theta =  atan(y/x);
  theta = (theta * 180) / pi;
  return theta;
}
