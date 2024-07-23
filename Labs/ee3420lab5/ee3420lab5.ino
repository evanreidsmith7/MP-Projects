/*
ee3420lab5.ino
date: 10/20/22
Written by: Evan smith
this program is designed to control a servo motor using the Servo libray.
The motor will be controled via user input from Serial, or a keypad input
connected to the Mega Aux Board
The ee3420_mega_aux_board.h header will need to be in the same directory as this file.
*/
//Servo libray
#include <Servo.h>
//commonly used libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//libray for mega aux board
#include <ee3420_mega_aux_board.h>

//initialize servo motor, and keypad input
Servo mservo;
char key_input[3]={0,0,0};
int pulse = 1500;
bool key = false;
void setup() 
{
//setting up serial connection and declaring the servo pin
//sets the pw to 1500microsecs
    Serial.begin(115200, SERIAL_8N1);
    mservo.attach(9);
    mservo.writeMicroseconds(1500);
    while (Serial.available()){Serial.read();}//clear
}
void loop() 
{
  //user input
  while (key)
  {
    char keypad_current_key = keypad.getKey();
    if (keypad_current_key != NO_KEY)
    {
      
      char input = keypad_current_key;
      Serial.println(input);
      controls(input);
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
    }
    if (Serial.available())
    {
      char input = Serial.read();
      controls(input);
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
      while (Serial.available()){Serial.read();}
    }
    if((key_input[0]=='*')&&(key_input[1]=='*')&&(key_input[2]=='*')) 
    {
      key = false;
      pulse = 1500;
    }
  }
      while (Serial.available()){Serial.read();}
      Serial.println("press a key to turn on");
      while (!Serial.available()){Serial.read();}
      key = true;
}
//this method acts as the controller for the servo
//it controls the pulse width
//written by: Evan Smith
void controls(char in)
{
  int pw = pulse;
  switch (in)
  {
    case '1':
      pw++;
      break;
    case '2':
      pw += 10;    
      break;
    case '3':
      pw+= 100;
      break;      
    case '4':
      pw--;
      break;
    case '5':
      pw -= 10;
      break;
    case '6':
      pw -= 100;
      break;
    case '7':
      pw = 1000;
      break;      
    case '8':
      pw = 1500;
      break;
    case '9':
      pw = 2000;
      break;
    case '0':
      for (int i = 1000; i < 2000; i++)
      {
        mservo.writeMicroseconds(i);
        delay(1);
      }
      for (int i = 2000; i > 1000; i--)
      {
        mservo.writeMicroseconds(i);
        delay(1);
      }     
      break;
  }
  if (pw >= 1000 && pw <= 2000)
  {
    pulse = pw;
  }
  else 
  {
    pw = pulse;
  }
  mservo.writeMicroseconds(pulse);
  Serial.println(pulse);
  Serial.println(pw);
  Serial.println("****"); 
}
  
