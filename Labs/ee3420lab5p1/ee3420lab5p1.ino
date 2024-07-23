#include <ASCII.h>
#include <ee3420_mega_aux_board.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SPEED 5 // Enable Pin for motor
#define CW 3 // Control pin 1 for motor 
#define CCW 4 // Control pin 2 for motor 
char key_input[3]={0,0,0};
int dCycle = 0;
bool key = true;
int velo = 0;
void setup() 
{
    Serial.begin(115200, SERIAL_8N1);
    pinMode(SPEED, OUTPUT);
    pinMode(CW, OUTPUT);
    pinMode(CCW, OUTPUT);
    while (Serial.available()){Serial.read();}//clear
}
 
void loop() 
{
 
  while (key)
  {
    char keypad_current_key = keypad.getKey();
    if (keypad_current_key != NO_KEY)
    {
      char input = keypad_current_key;
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
    }

  }
      while (Serial.available()){Serial.read();}
      Serial.println("press a key to turn back on");
      while (!Serial.available()){Serial.read();}
}

void controls(char in)
{
  switch (in)
  {
    case 'A':
      dCycle++;
      break;
    case 'B':
      dCycle--;
      break;
    case 'C':
      dCycle += 10;;
      break;      
    case 'D':
      dCycle -=10;
      break;
    case '0':
      dCycle = 0;
      velo = 0;
      break;
  }
  if (dCycle > 0 && dCycle <= 100)
  {
    velo = (255*dCycle)/100;
    digitalWrite(CW, HIGH);
    digitalWrite(CCW, LOW);
    //analogWrite(SPEED,225);
  }
  analogWrite(SPEED,velo);
  Serial.println(dCycle);
  Serial.println(velo);
  Serial.println("*******");
  
    
}
  
