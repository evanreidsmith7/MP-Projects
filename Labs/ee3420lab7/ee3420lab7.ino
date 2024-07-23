 /*
 * ee3420lab7.ino
 * Written by: Evan Smith
 * Example for controlling an 7 Segment LED display via 74HC595\
 * and 8x8 dot matrix using the max7219
*/

//commonly useful system libraries
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// this is a local library and should be located in the same folder as the source code
#include "ASCII.h"  

/* the ee3420_mega_aux_board.h header includes a lot of setup/shutdown functionality */
/* the header was written for this class and should be included in every lab program */
/* that involves the auxillary board */
#include "ee3420_mega_aux_board.h"  //comment out this line if not using aux board

/* the ee3420_mega_adc_board.h header includes a lot of setup/shutdown functionality */
/* the header was written for this class and should be included in every lab program */
/* that involves the analog-to-digital board */
#include "ee3420_mega_adc_board.h"  //comment out this line if not using adc board

#include <SPI.h>

//53 is default CS for Mega 2560
#define SEG_CS 2 
#define MAX7219_CS 53
//Assume bit order on 74HC595 is DP, G, F, E, D, C, B, A
int digit_pattern[16]= {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
int raw_x, raw_y;

//written by Dr. Stapleton
void write_74HC595(int value)
{
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(value);
  digitalWrite(SEG_CS,1);
  digitalWrite(SEG_CS,0);
  SPI.endTransaction();
}
//written by Dr. Stapleton
void MAX7219_write(int offset, int value)
{
  
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(MAX7219_CS,0);
  SPI.transfer(offset);
  SPI.transfer(value);
  digitalWrite(MAX7219_CS,1);
  SPI.endTransaction();
}
//written by Dr. Stapleton
void LED8x8_init()
{
  pinMode(MAX7219_CS,OUTPUT);     //enable CS for MAX7219 driver
  digitalWrite(MAX7219_CS,1);    //CS is active-low so disable for now
  MAX7219_write(9,0);       //for 8x8 LED, no decode on MAX7219
  MAX7219_write(0xA,0xF);   //select maximum brightness
  MAX7219_write(0xB,7);     //select all digits/columns to be displayed
  MAX7219_write(0xC,1);     //enable output
  MAX7219_write(0xF,0);     //disable test mode
}
//written by Dr. Stapleton
void LED8x8_clear()
{
  for(int i=1; i<=8; i++)
  {
    MAX7219_write(i,0);  //set all LEDs to 0 for off
  }
}

void setup() 
{
  #ifdef EE3420_MEGA_AUX_H
    ee3420_mega_aux_setup();
  #endif
  #ifdef EE3420_MEGA_ADC_H
    ee3420_mega_adc_setup();
  #endif
  //initialize the serial port that will communicate to the host computer over USB
  Serial.begin(115200, SERIAL_8N1);
  Serial.setTimeout(60000);
  
  //initialize the pin controlling the built-in LED on the Arduino MEGA 2560
  //usually, this is equivalent to pin 13
  pinMode(LED_BUILTIN, OUTPUT);  //built in LED on Arduino Mega is pin 13
  digitalWrite(LED_BUILTIN,0);   //turn built in LED off
  
  lcd.begin(16,2);
  lcd.clear();
  SPI.begin();
  pinMode(SEG_CS,OUTPUT);
  LED8x8_init();
  write_74HC595(0);
}
//end of setup()
char key_input[3]={0,0,0};
bool key = true;
void loop() 
{

  

  while (key)
  {
    char keypad_current_key = keypad.getKey();
    if (keypad_current_key != NO_KEY)
    {
      
      char input = keypad_current_key;
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
    }
    if (Serial.available())
    {
      char input = Serial.read();
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
      while (Serial.available()){Serial.read();}
    }
    if((key_input[0]=='*')&&(key_input[1]=='*')&&(key_input[2]=='*')) 
    {
      key = false;
    }
    potentiometerLetter();
    joystick();
  }
}  
//end of loop()

int cur = 1013;
//FUNCTIONS
void joystick()
{
  raw_x=analogRead(JOYSTICK_X);
  raw_y=analogRead(JOYSTICK_Y);
  
  set();
  lcd.print(raw_x);
  lcd.print(", ");
  lcd.print(raw_y);
  Serial.print(raw_x);
  Serial.print(", ");
  Serial.println(raw_y);
  Serial.println();
  
  int x = (raw_x/128)+1;
  int y = (raw_y/128);

  lcd.setCursor(0,1);
  lcd.print(x);
  lcd.print(", ");
  lcd.print(y);
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);
  Serial.println();
  
  y = 1 << y;
  
  Serial.print(x,BIN);
  Serial.print(", ");
  Serial.println(y,BIN);
  
  LED8x8_clear();
  MAX7219_write(x,y);
  Serial.println();
  Serial.println();
  Serial.println();
}
void set()
{
  lcd.clear();
  lcd.setCursor(0,0);
}
void potentiometerLetter()
{
  int sample = analogRead(POTENTIOMETER);
  int hex = sample / 64;
  Serial.println(hex);
  write_74HC595(digit_pattern[hex]);
}
