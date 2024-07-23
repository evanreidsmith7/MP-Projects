#ifndef EE3420_MEGA_AUX_H
#define EE3420_MEGA_AUX_H

//keypad device in Rexqualis kit has pins from left to right ...
//row 0, row 1, row 2, row 3, col 0, col 1, col 2, col 3

// Keypad library described here: https://playground.arduino.cc/Code/Keypad
#include <Keypad.h>
//variables needed for Keypad

#define KEYPAD_ROW0 22
#define KEYPAD_ROW1 23
#define KEYPAD_ROW2 24
#define KEYPAD_ROW3 25
#define KEYPAD_COL0 26
#define KEYPAD_COL1 27
#define KEYPAD_COL2 28
#define KEYPAD_COL3 29

const byte keypad_rows = 4; //four rows
const byte keypad_cols = 4; //three columns

char keypad_keys[keypad_rows][keypad_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

/*
 * //alternate keypad layout
 * char keypad_keys[keypad_rows][keypad_cols] = {
 *   {'1','2','3','A'},
 *   {'4','5','6','B'},
 *   {'7','8','9','C'},
 *   {'E','0','F','D'}
 * };
 */

//the following places the keypad on the end header of the Mega2560
byte keypad_rowPins[keypad_rows] = {KEYPAD_ROW0, KEYPAD_ROW1, KEYPAD_ROW2, KEYPAD_ROW3}; //connect to the row pinouts of the keypad
byte keypad_colPins[keypad_cols] = {KEYPAD_COL0, KEYPAD_COL1, KEYPAD_COL2, KEYPAD_COL3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keypad_keys), keypad_rowPins, keypad_colPins, keypad_rows, keypad_cols );

//LiquidCrystal library is described here: https://www.arduino.cc/en/Reference/LiquidCrystal

#include <LiquidCrystal.h>

int lcd_rows=2;
int lcd_cols=16;

#define LCD_RS 30
#define LCD_RW 31
#define LCD_E 32
#define LCD_D4 33
#define LCD_D5 34
#define LCD_D6 35
#define LCD_D7 36

//LiquidCrystal pin options ...
/*
 * LiquidCrystal(rs, enable, d4, d5, d6, d7)
 * LiquidCrystal(rs, rw, enable, d4, d5, d6, d7)
 * LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7)
 * LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7) 
 */

LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
//must include lcd.begin() in setup()

//needed for temporarily holding text to be sent to LCD
char lcd_text_string[40];
int lcd_current_row;
int lcd_current_col;

#define BUZZER 37

#define LED_GREEN 38
#define LED_YELLOW 39
#define LED_RED 40
#define LED_BLUE 41
#define LED_WHITE 42

#define MOTOR_PWM 43
#define MOTOR_CW 44
#define MOTOR_CCW 45

#include "Servo.h"

#define SERVO0 46
#define SERVO1 47
#define SERVO2 48
#define SERVO3 49

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

#include <SPI.h>

#define SPI_MISO 50
#define SPI_MOSI 51
#define SPI_SCLK 52
#define SPI_CS 53



void ee3420_mega_aux_setup()
{
	lcd.begin(lcd_cols,lcd_rows);  //needed to tell LiquidCrystal library the number of columns,rows on LCD
	
  pinMode(LED_BUILTIN,OUTPUT);  //built in LED on Arduino Mega is pin 13
  digitalWrite(LED_BUILTIN,0);  //turn built in LED off
  pinMode(LED_GREEN,OUTPUT);  
  digitalWrite(LED_GREEN,0);  
  pinMode(LED_YELLOW,OUTPUT);  
  digitalWrite(LED_YELLOW,0);  
  pinMode(LED_RED,OUTPUT);
  digitalWrite(LED_RED,0);  
  pinMode(LED_BLUE,OUTPUT);  
  digitalWrite(LED_BLUE,0);  
  pinMode(LED_WHITE,OUTPUT);  
  digitalWrite(LED_WHITE,0);  

  pinMode(BUZZER,OUTPUT);  
  digitalWrite(BUZZER,0);  

	pinMode(MOTOR_PWM,OUTPUT);  
  analogWrite(MOTOR_PWM,0);
  pinMode(MOTOR_CW,OUTPUT);  
  digitalWrite(MOTOR_CW,0);
  pinMode(MOTOR_CCW,OUTPUT);  
  digitalWrite(MOTOR_CCW,0);    
  
  servo0.attach(SERVO0);
  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo3.attach(SERVO3);
  
  servo0.writeMicroseconds(1500);
  servo1.writeMicroseconds(1500);
  servo2.writeMicroseconds(1500);
  servo3.writeMicroseconds(1500);    
  
  SPI.begin();
    
}

#endif //#ifndef EE3420_MEGA_AUX_H
