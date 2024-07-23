/*
 * ee3420lab2.ino
 * date: 21 September 2022
 * Written by: Evan
 * This program allows input and output from/to local serial and lcd
 * additionally allows for an external serial communication
 * This code is designed for use on the Arduino Mega 2560.
 * The ee3420_mega_aux_board.h header will need to be in the same directory as this file.
 */

//commonly useful system libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//libray for mega aux board
#include "ee3420_mega_aux_board.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200, SERIAL_8N1); //local serial
  Serial1.begin(115200); //need UART connection from other arduino to tx1 rx1
  
#ifdef EE3420_MEGA_AUX_H
  ee3420_mega_aux_setup();
#endif

}

void loop() {
  // put your main code here, to run repeatedly:
//initilizes variables needed for input;
  int i;
  char key_input[3]={0,0,0};
  char keypad_current_key;
  int loop_active;

  
  while(Serial.available()) {Serial.read();}  //clear the serial buffer
  Serial.println("Press any key in the Serial terminal to continue.");
  while(!Serial.available()) {}  //empty wait loop
  while(Serial.available()) {Serial.read();} //clear the serial buffer

  
  lcd.clear();
  
  loop_active=1;
  while(loop_active!=0)
  {  
    //keypad input  
    keypad_current_key=keypad.getKey();
    if (keypad_current_key != NO_KEY)//if something pressed this engages
    {
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]=keypad_current_key;

      //local serial
      Serial.println(keypad_current_key);
      Serial.print(key_input[0]);
      Serial.print(key_input[1]);      
      Serial.println(key_input[2]);
      Serial.println("---");      

      //print on local lcd
      lcd.setCursor(0,0);
      lcd.print(keypad_current_key);
      lcd.setCursor(0,1);
      lcd.print(key_input[0]);
      lcd.print(key_input[1]);      
      lcd.print(key_input[2]);

      //sends local keypad input to serial
      Serial1.write(keypad_current_key);
      Serial1.write(key_input[0]);
      Serial1.write(key_input[1]);      
      Serial1.write(key_input[2]);
      Serial1.write("---");
           
    } 
    //this stops input from all devices
    if((key_input[0]=='*')&&(key_input[1]=='*')&&(key_input[2]=='*')) 
    {
      while (Serial.available()){Serial.read();}
      char c;
      while (c != 'a'){c = Serial.read();}
      
      while (Serial.available()){Serial.read();}
    }

    //local serial input
    if (Serial.available())
    {
      
      char input = Serial.read();
      //store 3 vals
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
      //print to local serial
      Serial.println(input);
      Serial.print(key_input[0]);
      Serial.print(key_input[1]);      
      Serial.println(key_input[2]);
      Serial.println("---");

      //print to lcd
      lcd.setCursor(0,0);
      lcd.print(input);
      lcd.setCursor(0,1);
      lcd.print(key_input[0]);
      lcd.print(key_input[1]);      
      lcd.print(key_input[2]);

      //print to partner serial
      Serial1.write(input);
      Serial1.write(key_input[0]);
      Serial1.write(key_input[1]);      
      Serial1.write(key_input[2]);
      Serial1.write("---"); 
      //clear
      while(Serial.available()) {Serial.read();}
    }

    //this allows for external serial input
    if (Serial1.available())
    {
      
      char input1 = Serial1.read();
      //store 3 vals
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input1;
      //print to local serial
      Serial.println(input1);
      Serial.print(key_input[0]);
      Serial.print(key_input[1]);      
      Serial.println(key_input[2]);
      Serial.println("---");

      //print to lcd
      lcd.setCursor(0,0);
      lcd.print(input1);
      lcd.setCursor(0,1);
      lcd.print(key_input[0]);
      lcd.print(key_input[1]);      
      lcd.print(key_input[2]);
      
     
      
      //clear
      while(Serial1.available()) {Serial1.read();}
    }
    
    
  }

}
