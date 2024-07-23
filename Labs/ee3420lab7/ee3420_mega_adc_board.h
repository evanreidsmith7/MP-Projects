#ifndef EE3420_MEGA_ADC_H
#define EE3420_MEGA_ADC_H

/* For all analog sample values ... */
/* Voltage = (sample value/1024) * 5V   */
/* For millivolts ... */
/*  millivoltage = (sample value/1024) * 5000 mV   */

/* The ADC board has a space for a thermometer chip */
/* This is suitable for any one of LM34, LMM35, or MCP9701 */
/* Please uncomment the appropriate definition */
// #define LM34 A0  
/* LM34 voltage = 10mV * degrees F */
// #define LM35 A0  
/* LM35 voltage = 10mV * degrees C */
#define MCP9701 A0  
/* MCP9701 voltage = 400 mV + (19.5 mV * degrees C) */
#define ADC_TEMPERATURE A0

/* CdS Photocell in series with 10k resistor */
/* CdS between 5V and pin, 10k between pin and ground */
#define PHOTOCELL A1
/* Photocell resistance = 10 kohm * ((1024/sample value) - 1) */

/* Potentiometer */
#define POTENTIOMETER A2

/* Joystick */
#define JOYSTICK_BUTTON A3
#define JOYSTICK_Y A4
#define JOYSTICK_X A5
/* button probably better treated as digital input despite presence on ADC board */

/* Gas Sensor Analog Outout*/
#define GAS_SENSOR A6
/* Rexqualis reference says output >= 1V indicates "significant" gas levels */
/* no other information given about meaning of the output */

/* Microphone */
#define MICROPHONE A7

/* Rotary Encoder */
#define ENCODER_B A8
#define ENCODER_A A9
/* probably better treated as digital inputs despite presence on ADC board */

/* Pushbutton */
#define ADC_BUTTON A15
/* probably better treated as digital input despite presence on ADC board */
/* wired as active-high momentary pushbutton with 10k pulldown. */

void ee3420_mega_adc_setup()
{
  pinMode(JOYSTICK_BUTTON,INPUT);  //built in LED on Arduino Mega is pin 13
	pinMode(ENCODER_A,INPUT);  
  pinMode(ENCODER_B,INPUT);  
  pinMode(ADC_BUTTON,INPUT);  
  
}

float calculate_volts(int sample_value)
{
  float raw;
  float voltage;
  raw = (float)(sample_value);
  voltage = raw * 5.0 / 1024.0;
  return(voltage); 
}

float calculate_millivolts(int sample_value)
{
  float raw;
  float voltage;
  raw = (float)(sample_value);
  voltage = raw * 5000.0 / 1024.0;
  return(voltage); 
}

/* calculates correct Fahrenheit temperature (as float) from raw analog sample */
float calculate_LM34_fahrenheit(int sample_value)
{
  float fahrenheit;
  fahrenheit = calculate_millivolts(sample_value) / 10.0;
  return(fahrenheit); 
}

/* calculates correct Celsius temperature (as float) from raw analog sample */
float calculate_LM34_celsius(int sample_value)
{
  float fahrenheit;
  float celsius;
  fahrenheit = calculate_LM34_fahrenheit(sample_value);
  celsius = (fahrenheit - 32.0) * 5.0 / 9.0;
  return(celsius); 
}

/* calculates correct Celsius temperature (as float) from raw analog sample */
float calculate_LM35_celsius(int sample_value)
{
  float celsius;
  celsius = calculate_millivolts(sample_value) / 10.0;
  return(celsius); 
}

/* calculates correct Fahrenheit temperature (as float) from raw analog sample */
float calculate_LM35_fahrenheit(int sample_value)
{
  float fahrenheit;
  fahrenheit = (calculate_LM35_celsius(sample_value) * 9.0 / 5.0) + 32.0;
  return(fahrenheit); 
}

/* calculates correct Celsius temperature (as float) from raw analog sample */
float calculate_MCP9701_celsius(int sample_value)
{
  float celsius;
  celsius = (calculate_millivolts(sample_value) - 400.0) / 19.5;
  return(celsius); 
}

/* calculates correct Fahrenheit temperature (as float) from raw analog sample */
float calculate_MCP9701_fahrenheit(int sample_value)
{
  float fahrenheit;
  fahrenheit = (calculate_MCP9701_celsius(sample_value) * 9.0 / 5.0) + 32.0;
  return(fahrenheit); 
}

float calculate_photocell_resistance(int sample_value)
{
  float raw;
  float millivoltage;
  float resistance;
  raw = (float)(sample_value);
  if(raw == 0.0) {raw = 0.5;}
  resistance = 10000.0 *((1024.0/raw)-1.0);
  return(resistance);
}

#endif //#ifndef EE3420_MEGA_ADC_H
