/*
   ee3420lab4.ino
   date: 10/6/22
   Written by: Evan smith
   this program is designed to control a set of traffic lights for the north and south
   direction. Another micro processor will be connected by uart to control the EW lights
   this program acts as a host to the other Arduino that controls the EW lights.
   A blue light is also added to the lights for an emergancy overide which stalls
   traffic.
   The ee3420_mega_aux_board.h header will need to be in the same directory as this file.
*/
//commonly used libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//libray for mega aux board
#include <ee3420_mega_aux_board.h>

int blue = 13;
int green = 12;
int yellow = 11;
int red = 10;
char input = 0;
bool key = true;
char key_input[3]={0,0,0};
char over = false;
int state = 1;
void setup() 
{
  Serial.begin(115200, SERIAL_8N1); //local serial
  Serial1.begin(115200); //external serial
  //light setup
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(yellow,OUTPUT);
  pinMode(red,OUTPUT);
  
  #ifdef EE3420_MEGA_AUX_H
  ee3420_mega_aux_setup();
  #endif

  
}

void loop() 
{
  //input handling loop while key is on
  while (key)
  {
    char keypad_current_key = keypad.getKey();
    if (keypad_current_key != NO_KEY)
    {
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]=keypad_current_key;
      over = true;
    }
    if (Serial.available())
    {
      input = Serial.read();
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]=input;
      over = true;
      while (Serial.available()){Serial.read();}
    }
    if (Serial1.available())
    {
      input = Serial1.read();
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]=input;
      over = true;
      while (Serial.available()){Serial.read();}
    }
    if((key_input[0]=='*')&&(key_input[1]=='*')&&(key_input[2]=='*')) 
    {
      key = false;
    }
      machine(state);
      Serial.print(key_input[2]);
      Serial.print(key_input[1]);
      Serial.println(key_input[0]);

  }
  while (Serial.available()){Serial.read();}
  while (!Serial.available())
  {
    nsLights(0,0,0,0);
    Serial1.write(10);
    
  }
  key = true;
  
}
//this is the method which input is sent to and acts as a state machine
//parameters: in, input 
//written by Evan Smith
void machine (int in)
{
  if (over)
    Serial.println("emergancy services overrides");
  else
    Serial.println("override is off");
    
  switch (in)
  {
    case 1:
      control(1);
      delay(3000);
      if (over == true)
      {
        state = 9;
      }
      else
      {
        state = 2;
      }
      break;
    case 2:
      control(2);
      if (over == true)
      {
        delay(2000);
        state = 9;
      }
      else
      {
        delay(7000);
        state = 3;//dummy
      }
      break;
    case 3:
      control(3);
      delay (2000);
      if (over)
        state = 9;
      else
        state = 4;
      break;
    case 4:
      control(4);
      delay (3000);
      if (over)
        state = 9;
      else
        state = 5;      
      break;
    case 5:
      control(5);
      if (over)
      {
        delay(2000);
        state = 8;
      }
      else
      {
        delay(7000);
        state = 6;
      }
      break;
    case 6:
      control(6);
      delay(2000);
      if (over)
        state = 9;
      else
        state = 1;
      break;
    case 7:
      control(7);
      delay(2000);
      state = 9;
      break;
    case 8:
      control(8);
      delay(2000);
      state = 9;
      break; 
    case 9:
      control(9);
      for (int i = 0; i < 5; i++)
      {
        //digitalWrite(BUZZER,1);
        delay(1000); //1000 ms = 1s
        //digitalWrite(BUZZER,0);
        delay(1000); //1000 ms = 1s
      }
      over = false;
      state = 1;
      break;      
  }
}
//this method is for the light control
//Written by Evan Smith
// parameters: b,g,y,r are the led pins that match their color
void nsLights(int b,int g,int y,int r)
{
    char keypad_current_key = keypad.getKey();
    if (keypad_current_key != NO_KEY)
    {
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]=keypad_current_key;
      over = true;
    }
    if (Serial.available())
    {
      input = Serial.read();
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]=input;
      over = true;
      while (Serial.available()){Serial.read();}
    }
    if (Serial1.available())
    {
      input = Serial1.read();
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]=input;
      over = true;
      while (Serial.available()){Serial.read();}
    }
  digitalWrite(blue,b);
  digitalWrite(green,g);
  digitalWrite(yellow,y);
  digitalWrite(red,r);
}
//this method takes input from the machine function and puts the lights into the current
//state. it also sends the current state to the external serial
//parameter: in, input that is the state
//written by Evan Smith
void control(int in)
{
  Serial.print("state ");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(in);
  switch (in)
    {
      case 1:
        Serial.println(in);
        nsLights(0,0,0,1);
        Serial1.write(in);
        break;
      case 2:
        Serial.println(in);
        nsLights(0,1,0,0);
        Serial1.write(in);
        break;
      case 3:
        Serial.println(in);
        nsLights(0,0,1,0);
        Serial1.write(in);
        break;
      case 4:
        Serial.println(in);
        nsLights(0,0,0,1);
        Serial1.write(in);
        break;
      case 5:
        Serial.println(in);
        nsLights(0,0,0,1);
        Serial1.write(in);
        break;
      case 6:
        Serial.println(in);
        nsLights(0,0,0,1);
        Serial1.write(in);
        break;
      case 7:
        Serial.println(in);
        nsLights(0,0,1,0);
        Serial1.write(in);
        break;
      case 8:
        Serial.println(in);
        nsLights(0,0,0,1);
        Serial1.write(in);
        break;
      case 9:
        Serial.println(in);
        nsLights(1,0,0,1);
        Serial1.write(in);
        break;
    }
}
