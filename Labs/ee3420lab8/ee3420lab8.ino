/*
ee3420lab5.ino
date: 10/31/22
Written by: Evan smith
this program is a real time clock that is capable of storing 
memory to a i2ceeprom
*/
#include "ASCII.h"
//libray for mega aux board
#include "ee3420_mega_aux_board.h"
//commonly useful system libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
//libray for mega adc board
//#include "ee3420_mega_adc_board.h"
#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"

RTC_DS3231 rtc;

DateTime rtc_now, last_action;

const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char monthNames[12][12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October" , "November", "December"};

int temp;
int current_year, current_month, current_day, current_hour, current_minute, current_second;
float current_temperature;
char charBuffer[80];

#include "Adafruit_EEPROM_I2C.h"
  
Adafruit_EEPROM_I2C i2ceeprom;

#define EEPROM_ADDR 0x57  // address range is 0x50 - 0x57


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
  Serial.setTimeout(60000);
  lcd.begin(16,2);
  Wire.begin();
  Wire.setClock(100000);


//note DS3231 requires Wire so Wire must be initialized first
  if (! rtc.begin()) 
  {
    Serial.println(F("Couldn't find RTC"));
    Serial.println(F("Suspending operation ..."));
    Serial.flush();
    while (1) delay(10);
  }
  
  while(Serial.available()) {Serial.read();}  //clear any serial input from buffer 

  if (rtc.lostPower()) 
  {
    prompt_and_set_RTC();
  }
  
  if (!i2ceeprom.begin(EEPROM_ADDR)) 
  {  
    Serial.print(F("IIC EEPROM not found at IIC address 0x"));
    Serial.println(EEPROM_ADDR, HEX);
    Serial.println(F("Suspending operation ..."));
    Serial.flush();
    while (1) delay(10);   
  } 

  rtc_now=rtc.now();
  last_action=rtc_now;
}
//end of setup


char key_input[3]={0,0,0};
bool key = true;
void loop()
{
  rtc_now = rtc.now();

  printTime();
  prompt();
  // user input
  while (key)
  {
    char keypad_current_key = keypad.getKey();
    if (keypad_current_key != NO_KEY)
    {
      char input = keypad_current_key;
      Serial.println(input);
      menu(input);
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
    }
    if (Serial.available())
    {
      char input = Serial.read();
      Serial.println(input);
      menu(input);
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
}
//end of loop



void increaseMinute()
{
  if(current_minute == 59)
  {
    current_minute = 0;
    increaseHour();
  }
  else
  {
    current_minute++;
  }
}
void increaseHour()
{
  if(current_hour == 23)
  {
    current_hour = 0;
    increaseDay();
  }
  else
  {
    current_hour++;
  }
}
void increaseDay()
{
  if(current_month == 2)
  {
    if(current_day == 28)
    {
      current_day = 0;
      increaseMonth();
    }
    else
    {
      current_day++;
    }
  }
  else if (current_month == 4 || current_month == 6 || current_month == 9 || current_month == 11)
  {
    if(current_day == 30)
    {
      current_day = 0;
      increaseMonth();
    }
    else
    {
      current_day++;
    }
  }
  else
  {
    if(current_day == 31)
    {
      current_day = 0;
      increaseMonth();
    }
    else
    {
      current_day++;
    }
  }
}
void increaseMonth()
{
  if (current_month == 12)
  {
    current_month = 0;
    current_year++;
  }
  else
  {
    current_month++;
  }
}
void decreaseMinute()
{
  if(current_minute != 0)
  {
    current_minute--;
  }
  else
  {
    current_minute == 59;
    decreaseHour();
  }
}
void decreaseHour()
{
  if(current_hour != 0)
  {
    current_hour--;
  }
  else
  {
    current_hour = 23;
    decreaseDay();
  }
}
void decreaseDay()
{
  if(current_day != 1)
  {
    current_day--;
  }
  else
  {
    if(current_month == 3)
    {
      current_day = 28;
      decreaseMonth();
    }
    else if(current_month == 5 || current_month == 7 || current_month == 10 || current_month == 12)
    {
      current_day = 30;
      decreaseMonth();
    }
    else
    {
      current_day = 31;
      decreaseMonth();
    }
  }
}
void decreaseMonth()
{
  if (current_month != 1)
  {
    current_month--;
  }
  else
  {
    current_month = 12;
    current_year--;
  }
}

  
  



void menu(char in)
{
  rtc_now=rtc.now();
  set();
  switch(in)
  {
    case '1':
    {
      prompt_and_set_RTC();
      break;
    }
    case '2':
    {
      increaseHour();
      Serial.println("adding an hour... UNSAVED TIME: ");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      printTime();
      break;
    }
    case '3':
    {
      decreaseHour();
      Serial.println("decreasing an hour... UNSAVED TIME: ");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      printTime();
      break;
    }
    case '4':
    {
      increaseMinute();
      Serial.println("adding a minute... UNSAVED TIME: ");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      printTime();
      break;
    }
    case '5':
    {
      decreaseMinute();
      Serial.println("decreasing a minute... UNSAVED TIME: ");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      printTime();
      break;
    }
    case '0':
    {
      current_second = 0;
      Serial.println("seconds to 0... UNSAVED TIME: ");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      printTime();
      break;
    }
    case 'A': //wirte to eeprom
    {
      writeEEPROM();
      break;
    }
    case 'a': //wirte to eeprom
    {
      writeEEPROM();
      break;
    }
    case 'B': //read data at eeprom
    {
      readEEPROM(); 
      break;
    }
    case 'b': //read data at eeprom
    {
      readEEPROM(); 
      break;
    }
    case 'C': //read data at eeprom
    {
      printTime(); 
      break;
    }
    case 'c': //read data at eeprom
    {
      printTime(); 
      break;
    }
  }
  rtc_now=rtc.now();
  prompt();
}
void set()
{
  current_year = rtc_now.year();
  current_month =rtc_now.month();
  current_day = rtc_now.day();
  current_hour = rtc_now.hour();
  current_minute = rtc_now.minute();
  current_second = rtc_now.second();
}
void readEEPROM()
{
  Serial.println(F("String located at I2C EEPROM offset 0x20"));
  i2ceeprom.read(0x20, charBuffer, 32); //days of the week
  Serial.println((String)charBuffer); 
  
  Serial.println(F("String located at I2C EEPROM offset 0x40"));
  i2ceeprom.read(0x40, charBuffer, 32); //time
  Serial.println((String)charBuffer);    
}
void writeEEPROM()
{
  rtc_now = rtc.now();
  sprintf(charBuffer,"%s, %s %d, %d ", daysOfTheWeek[rtc_now.dayOfTheWeek()], monthNames[rtc_now.month()-1],rtc_now.day(), rtc_now.year());
  Serial.println(F("Writing this new string to offset 0x20:")); 
  Serial.println(charBuffer);
  i2ceeprom.write(0x20, charBuffer, strlen(charBuffer)+1);
  
  sprintf(charBuffer,"%d:%02d:%02d %c.M.", rtc_now.twelveHour(), rtc_now.minute(), rtc_now.second(), (rtc_now.isPM()?'P':'A'));
  Serial.println(F("Writing this new string to offset 0x40:")); 
  Serial.println(charBuffer);
  i2ceeprom.write(0x40, charBuffer, strlen(charBuffer)+1);
}
void prompt()
{
  Serial.println();
  Serial.println("1: set time");
  Serial.println("2: add hour to time");
  Serial.println("3: subtract hour to time");
  Serial.println("4: add minute to time");
  Serial.println("5: subtract minute to time");
  Serial.println("0: set seconds to zero");
  Serial.println("A: save current date and time");
  Serial.println("B: read last saved date and time");
  Serial.println("C: print time");
  Serial.println();
}
void prompt_and_set_RTC()
{
    while(Serial.available()) {Serial.read();}  //clear any serial input from buffer 
        
    temp=-1;
    while(temp <2000 || temp > 2099)
    {
    Serial.println(F("Enter the current year as 2000-2099 as 4 digits: "));
    temp=Serial.parseInt();
    Serial.print(F("\nYou entered: "));
    Serial.println(temp);
    }
    Serial.print(F("Storing year as "));
    Serial.println(temp);
    current_year=temp;
     
    temp=-1;
    while(temp <1 || temp > 12)
    {
    Serial.println(F("Enter the current month as 1-12 with 1 as January and 12 as December: "));
    temp=Serial.parseInt();
    Serial.print(F("\nYou entered: "));
    Serial.println(temp);
    }
    Serial.print(F("Storing month as "));
    Serial.println(temp);
    current_month = temp;  
  
    temp=-1;
    while(temp <1 || temp > 31)
    {
    Serial.println(F("Enter the current day of the month as 1-31: "));
    temp=Serial.parseInt();
    Serial.print(F("\nYou entered: "));
    Serial.println(temp);
    }
    Serial.print(F("Storing day of the month as "));
    Serial.println(temp);
  
    current_day = temp ;  
  
    temp=-1;
    while(temp <0 || temp > 23)
    {
    Serial.println(F("Enter the current hour as 0-23: "));
    temp=Serial.parseInt();
    Serial.print(F("\nYou entered: "));
    Serial.println(temp);
    }
    Serial.print(F("Storing hour as "));
    Serial.println(temp);
  
    current_hour = temp ;  
  
    temp=-1;
    while(temp <0 || temp > 59)
    {
    Serial.println(F("Enter the current minute as 0-59: "));
    temp=Serial.parseInt();
    Serial.print(F("\nYou entered: "));
    Serial.println(temp);
    }
    Serial.print(F("Storing minute as "));
    Serial.println(temp);
  
    current_minute = temp ;  
  
    temp=-1;
    while(temp <0 || temp > 59)
    {
    Serial.println(F("Enter the current second as 0-59: "));
    temp=Serial.parseInt();
    Serial.print(F("\nYou entered: "));
    Serial.println(temp);
    }
    Serial.print(F("Storing second as "));
    Serial.println(temp);
  
    current_second = temp ;  //struct tm stores minute as 0-59
  
    while(Serial.available()) {Serial.read();}  //clear any serial input from buffer
  
#ifdef DEBUG
    Serial.println(F("\nTime entry summary:"));
    Serial.print(F("Year: "));
    Serial.println(current_year);
    Serial.print(F("Month: "));
    Serial.println(current_month);  
    Serial.print(F("Day: "));
    Serial.println(current_day);  
    Serial.print(F("Hour: "));
    Serial.println(current_hour);
    Serial.print(F("Minute: "));
    Serial.println(current_minute);  
    Serial.print(F("Second: "));
    Serial.println(current_second);  
#endif //#ifdef DEBUG
  
#ifdef DEBUG
    Serial.println(F("\nWriting time to DS3231")); 
#endif //#ifdef DEBUG
  
    rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
}
void bug()
{
  Serial.println(F("\nTime entry summary:"));
  Serial.print(F("Year: "));
  Serial.println(rtc_now.year());
  Serial.print(F("Month: "));
  Serial.println(rtc_now.month());  
  Serial.print(F("Day: "));
  Serial.println(rtc_now.day());  
  Serial.print(F("Hour: "));
  Serial.println(rtc_now.hour());
  Serial.print(F("Minute: "));
  Serial.println(rtc_now.minute());  
  Serial.print(F("Second: "));
  Serial.println(rtc_now.second());
}
void printTime()
{
  rtc_now = rtc.now();
  sprintf(charBuffer,"%s, %s %d, %d ", daysOfTheWeek[rtc_now.dayOfTheWeek()], monthNames[rtc_now.month()-1],rtc_now.day(), rtc_now.year());
  Serial.println(charBuffer);;
  
  sprintf(charBuffer,"%d:%02d:%02d %c.M.", rtc_now.twelveHour(), rtc_now.minute(), rtc_now.second(), (rtc_now.isPM()?'P':'A'));
  Serial.println(charBuffer);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(charBuffer);
}
