/*
 * ee3420lab8.ino
 * Written by: Evan Smith
 */

//commonly useful system libraries
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ASCII.h"  
#include "ee3420_mega_aux_board.h"
#include "ee3420_mega_adc_board.h"
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#include "SdFat.h"

#define SD_CS_PIN SS
File myFile;
SdFat SD;

RTC_DS3231 rtc;

DateTime rtc_now, last_action;

const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char monthNames[12][12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October" , "November", "December"};
int temp;
int current_year, current_month, current_day, current_hour, current_minute, current_second;
float current_temperature;
float airVolts;
float photoRes;
char scurrent_temperature[10];
char sairVolts[10];
char sphotoRes[10];
char charBuffer[80];

char sendBuffer[110];
char dateBuffer[30];
char timeBuffer[30];
int clock_ready=false;
float raw_sample;
char key_input[3]={0,0,0};
bool key = true;
const int interruptPin = 2;
int sampleCount = 0;

/*
 * START SETUP START SETUP START SETUP START SETUP START SETUP START SETUP START SETUP
 */
void setup() 
{
  #ifdef EE3420_MEGA_AUX_H
    ee3420_mega_aux_setup();
  #endif
  #ifdef EE3420_MEGA_ADC_H
    ee3420_mega_adc_setup();
  #endif
  
  Serial.begin(9600, SERIAL_8N1);
  Serial1.begin(9600, SERIAL_8N1);
  Serial.setTimeout(60000);
  SPI.begin();
  lcd.begin(16,2);
  lcd.clear();
  Wire.begin();
  Wire.setClock(100000);
  
  if (! rtc.begin()) 
  {
    Serial.println(F("Couldn't find RTC"));
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) 
  {
    prompt_and_set_RTC();
  }
  pinMode(interruptPin, INPUT_PULLUP);
  rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
  attachInterrupt(digitalPinToInterrupt(interruptPin), clock_action, FALLING);
  while(Serial.available()) {Serial.read();}  //clear any serial input from buffer
}
/*
 * END SETUP END SETUP END SETUP END SETUP END SETUP END SETUP END SETUP END SETUP
 */

/*
 * START LOOP START LOOP START LOOP START LOOP START LOOP START LOOP START LOOP
 */
void clock_action()
{
  clock_ready=true;
}
void loop() 
{
  while (key)
  {
    char keypad_current_key = keypad.getKey();
    if (keypad_current_key != NO_KEY)
    {
      
      char input = keypad_current_key;
      if(input == 'A')
      {
        prompt();
      }
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
    }
    if (Serial.available())
    {
      char input = Serial.read();
      if(input == 'A')
      {
        prompt();
      }
      key_input[2]=key_input[1];
      key_input[1]=key_input[0];
      key_input[0]= input;
      while (Serial.available()){Serial.read();}
    }
    if((key_input[0]=='*')&&(key_input[1]=='*')&&(key_input[2]=='*')) 
    {
      key = false;
      temperature();
      gas();
      photo();
      updateCharBuffer();
      //sprintf(sendBuffer,"%s°F, %sV, %sKOhms, %s, %s", scurrent_temperature, sairVolts, sphotoRes, "-1", timeBuffer);
      sprintf(sendBuffer,"%s, %s, %s, %s, %s", scurrent_temperature, sairVolts, sphotoRes, "-1", timeBuffer);
      Serial.println(sendBuffer);
      Serial1.write(sendBuffer);
      Serial1.write('\n');
      Serial.println();
      Serial.println("above is the last sample sent to other mp");
    }
    if(clock_ready)
    {
      clock_ready=false;
      if (key)
      {
        temperature();
        gas();
        photo();
        updateCharBuffer();
  
        Serial.println("line below will be sent to other arduino for now");
        sprintf(sendBuffer,"%s, %s, %s, %s, %s", scurrent_temperature, sairVolts, sphotoRes, dateBuffer, timeBuffer);
        Serial.println(sendBuffer);
        Serial1.write(sendBuffer);
        Serial1.write('\n');
        Serial.println();
      }
    }    
  }
  
}
/*
 * END LOOP END LOOP END LOOP END LOOP END LOOP END LOOP END LOOP END LOOP
 */

 
void updateCharBuffer()
{
  DateTime now = rtc.now();
  sprintf(charBuffer,"%s, %s %d, %d %d:%02d:%02d %c.M.", daysOfTheWeek[now.dayOfTheWeek()], monthNames[now.month()-1],now.day(), now.year(), now.twelveHour(), now.minute(), now.second(), (now.isPM()?'P':'A'));
  //Serial.println(charBuffer);
  sprintf(dateBuffer,"%s %s %d %d", daysOfTheWeek[now.dayOfTheWeek()], monthNames[now.month()-1],now.day(), now.year());
  sprintf(timeBuffer,"%d:%02d:%02d %c.M.", now.twelveHour(), now.minute(), now.second(), (now.isPM()?'P':'A'));

}
void photo()
{
  lcd.setCursor(0,1);
  raw_sample=analogRead(PHOTOCELL);
  photoRes = calculate_photocell_resistance(raw_sample)/ 1000;
  dtostrf(photoRes, 3, 2,sphotoRes);
  //Serial.print("Photo Resistance: ");
  //Serial.print(photoRes);
  //Serial.println("Kohms");
  lcd.print("Light:");
  lcd.print(photoRes);
  lcd.print("Kohms");
}
void temperature()
{
  lcd.clear();
  lcd.setCursor(0,0);
  raw_sample=analogRead(ADC_TEMPERATURE);
  current_temperature = calculate_MCP9701_fahrenheit(raw_sample);
  dtostrf(current_temperature, 4, 2,scurrent_temperature);
  //Serial.print(calculate_MCP9701_fahrenheit(raw_sample));
  //Serial.println("°F");
  lcd.print(calculate_MCP9701_fahrenheit(raw_sample));
  lcd.print("\xDF""F"); 
}
void gas()
{
  raw_sample = analogRead(GAS_SENSOR);
  airVolts = calculate_volts(raw_sample);
  dtostrf(airVolts, 3, 2,sairVolts);
  //Serial.print("gas level ");
  //Serial.println(calculate_volts(raw_sample));
  lcd.print("gas:");
  lcd.print(airVolts);
  lcd.print("V");
  if (airVolts <= 0.50)
  {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
  }
  if (airVolts > 0.50 && airVolts <= 1.00)
  {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
  }
  if (airVolts > 1.00)
  {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
  }
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
  
  rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
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
      Serial.println("adding an hour...");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      outTime();
      break;
    }
    case '3':
    {
      decreaseHour();
      Serial.println("decreasing an hour...");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      outTime();
      break;
    }
    case '4':
    {
      increaseMinute();
      Serial.println("adding a minute...");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      outTime();
      break;
    }
    case '5':
    {
      decreaseMinute();
      Serial.println("decreasing a minute...");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      outTime();
      break;
    }
    case '0':
    {
      current_second = 0;
      Serial.println("seconds to 0...");
      rtc.adjust(DateTime(current_year, current_month, current_day, current_hour, current_minute, current_second));
      outTime();
      break;
    }
    case 'C': //print
    {
      outTime(); 
      break;
    }
    case 'c': //print
    {
      outTime(); 
      break;
    }
  }
  rtc_now=rtc.now();
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
void outTime()
{
  DateTime now = rtc.now();
  sprintf(charBuffer,"%s, %s %d, %d %d:%02d:%02d %c.M.", daysOfTheWeek[now.dayOfTheWeek()], monthNames[now.month()-1],now.day(), now.year(), now.twelveHour(), now.minute(), now.second(), (now.isPM()?'P':'A'));
  Serial.println(charBuffer);
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
  Serial.println("C: print time");
  Serial.println();
  
  while(Serial.available()){Serial.read();}
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
}






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
