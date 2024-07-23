//#include <BlockDriver.h>
#include <FreeStack.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <sdios.h>
//#include <SysCall.h>
#include <string.h>
#include <SPI.h>

SdFat SD;
File myFile;
#include <SoftwareSerial.h>
SoftwareSerial softSerial(2, 3); // 2=RX, 3=TX

//LED pin
int LED = 13;
int sampleCount = 0;
#define SD_CS_PIN SS



void setup()
{
  softSerial.begin(9600);
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  while (softSerial.available())
  {
    softSerial.read();
  }


  Serial.println(F("\nSD File IO Demonstration\n\n"));
  while (Serial.available())
  {
    Serial.read(); //clear any serial input from buffer
  }
  Serial.print(F("Initializing SD card... "));
  if (!SD.begin(SD_CS_PIN))
  {
    Serial.println(F("SD initialization failed!"));
    return;
  }
  Serial.println(F("SD initialization done."));

 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile)
  {
    Serial.print(F("Writing to test.txt..."));
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else
  {
    // if the file didn't open, print an error:
    Serial.println(F("error opening test.txt for writing"));
  }
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile)
  {
    Serial.println("test.txt:");
    // read from the file until there's nothing else in it:
    while (myFile.available())
    {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println(F("error opening test.txt for reading"));
  }

}


const unsigned int MAX_MESSAGE_LENGTH = 100;

char sBuffer[MAX_MESSAGE_LENGTH];
bool fo = true;
void loop() {

  //Check to see if anything is available in the serial receive buffer
  while (softSerial.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = softSerial.read();

    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    //Full message received...
    else
    {
      while (softSerial.available())
      {
        softSerial.read(); //clear any serial input from buffer
      }
      //Add null character to string
      message[message_pos] = '\0';
      //Print the message (or do other things)
      Serial.print("data recieved: ");
      Serial.println(message);
      Serial.println();
      static int i = 0;

      check(message);
      
      if (sampleCount < 61)
      {
        Serial.println(F("Opening weather.csv")); //change to weather
        myFile = SD.open("weather.csv", FILE_WRITE);
        if (!myFile)
        {
          Serial.println(F("Unable to open CSV file.  Suspending."));
          Serial.flush();
          while (1);
        }
        Serial.print("data to sd: ");
        Serial.println(message);
        myFile.println(message);
        Serial.println(sampleCount);
        sampleCount++;
        myFile.close();
        Serial.println(F("Data file closed."));
        Serial.println();
        Serial.println();
        myFile.close();
      /*else
        {
          myFile.close();
          myFile = SD.open("mytestdata.csv");
          if (myFile)
          {
            Serial.println("mytestdata.csv");
            // read from the file until there's nothing else in it:
            while (myFile.available())
            {
              Serial.write(myFile.read());
            }
          }
          Serial.flush();
          Serial.println(F("Data file closed.  Press any key to reopen for more samples."));
          while (!Serial.available());
          while (Serial.available())
          {
            Serial.read();
          }
        }
        */    
        //Reset for the next message
      }
      message_pos = 0;
    }
  }
}
void check(char n[])
{
  char m[strlen(n) + 10];  
  strcpy(m, n);
  int i = 0;
  while (m[i]!= '\0')
  {
    if (m[i] == '-' && m[i+1] == '1')
    {
      fo = false;
      while(1)
      {
        
      }
      while (softSerial.available())
      {
        softSerial.read(); //clear any serial input from buffer
      }
    }
    i++;
    break;
  }
}
