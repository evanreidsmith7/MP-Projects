/*
   ee3420lab3.ino
   date: 28 September 2022
   Written by: Evan
   This program allows for the control of stepper motor with three modes
   This code is designed for use on the Arduino Mega 2560.
   The ee3420_mega_aux_board.h header will need to be in the same directory as this file.
*/
//commonly used libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ASCII.h>
#include <ee3420_mega_aux_board.h>

#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

const int maxSteps = 3;
bool dir = true;
int mode = 1;
char input = 1;
int rate = 100;
int stepNum1 = 0;
int stepNum2 = 0;
int stepNum3 = 0;
bool key = true;
void setup()
{
  // put your setup code here, to run once:
  //sets up the Serial connection to board and stepper motor pins
  Serial.begin(115200, SERIAL_8N1);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
}

void loop()
{

  if (key)
  {
    if (mode == 1)
    {
      singleCoil(); //modes
      delay(rate);
    }
    else if (mode == 2)
    {
      doubleCoil();
      delay(rate);
    }
    else if (mode == 3)
    {
      halfStep();
      delay(rate);
    }
  }

  char keypad_current_key = keypad.getKey();
  if (keypad_current_key != NO_KEY)
  {
    input = keypad_current_key;
    controls(input);
  }
  if (Serial.available())
  {
    input = Serial.read();
    controls(input);
    while (Serial.available()){Serial.read();}
  }
}
//Author: Evan Smith, controls is where input is taken and thrown to a switch case
//which acts like a controller for the motor
void controls(char in)
{
  switch (in)
  {
    case 'A':
      dir  = true;
      Serial.println("Changing to clock-wise direction");
      break;
    case 'B':
      dir = false;
      Serial.println("Changing to counter clock-wise direction");
      break;
    case 'C':
      if (rate < 1000 )
      {
        rate = rate + 10;
        Serial.println("Decreasing number of steps per second");
      }
      else
      {
        Serial.println("1000 limit reached (1 step per sec)... slow");
      }
      break;
    case 'D':
      if (rate > 1)//1000 steps per second4
      {
        rate = rate - 10;
        Serial.println("increasing number of steps per second");
      }
      else
      {
        Serial.println("1 limit reached (1000 step per sec)... fast");
      }
      break;
    case '*':
      Serial.println("STOP!");
      key = false;
      break;
    case 'E':
      Serial.println("STOP!");
      key = false;
      break;
    case '#':
      Serial.println("GO!");
      key = true;
      break;
    case 'F':
      Serial.println("GO!");
      key = true;
      break;
    case '1':
      Serial.println("SINGLE COIL FULL MODE!");
      mode = 1;
      break;
    case '2':
      Serial.println("DUAL COIL FULL MODE!");
      mode = 2;
      break;
    case '3':
      Serial.println("HLF STEP MODE!");
      mode = 3;
      break;
  }
}
//this is the state machine for single coil full step control
void singleCoil() //full
{
   if(dir == true)
   {
    switch(stepNum1)
    {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break; 
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;     
    }
   }
   else
   {
    switch(stepNum1)
    {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break; 
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);    
        break;     
    }    
   }

  stepNum1++; 

  if(stepNum1 > 3)
  {
    stepNum1 = 0;
  }
  
}
//this is the state machine for double coil full step control
void doubleCoil() //full
{
   if(dir == true)
   {
    switch(stepNum2)
    {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break; 
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;
      case 4:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);    
        break;        
    }
   }
   else
   {
    switch(stepNum2)
    {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break; 
      case 1:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, HIGH);    
        break;     
      case 4:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
    }    
   }

  stepNum2++;

  if(stepNum2 > 4)
  {
    stepNum2 = 0;
  }
  
}
//this is the state machine for half step control
void halfStep() //full
{
   if(dir == true)
   {
    switch(stepNum3)
    {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break; 
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;
      case 4:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;
      case 5:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;
      case 6:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;
      case 7:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);    
        break;        
    }
   }
   else
   {
    switch(stepNum3)
    {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break; 
      case 1:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;
      case 4:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;
      case 5:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;
      case 6:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);    
        break;
      case 7:
        digitalWrite(STEPPER_PIN_1, LOW); 
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, HIGH);    
        break;
    }
   }    
   

  stepNum3++; 

  if(stepNum3 > 7)
  {
    stepNum3 = 0;
  }
   
  
}
