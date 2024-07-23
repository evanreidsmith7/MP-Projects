# EE 3420 Microprocessors Labs

This repository contains the laboratory projects for the EE 3420 Microprocessors class at Texas State University. Each lab is implemented using the Arduino Mega 2560 and associated peripherals.

## Labs

### Lab 1: UART Communication – Perfect Triangle Generator

- **Description**: This lab involves creating an interactive console interface through the UART to find "perfect" triangles, where all three sides have integer lengths.
- **Files**:
  - [Lab 1 Code](./Labs/ee3420lab1/ee3420lab1.ino)

### Lab 2: General-Purpose Input-Output (GPIO) – Keypad and Character LCD

- **Description**: Extend user input and output options using a 4x4 telephone-style keypad and character LCD display. Also involves communication using a serial connection to another microprocessor.
- **Files**:
  - [Lab 2 Code](./Labs/ee3420lab2/ee3420lab2.ino)
  - [ASCII Header](./Labs/ee3420lab2/ASCII.h)
  - [Aux Board Header](./Labs/ee3420lab2/ee3420_mega_aux_board.h)
  - [Keypad Header](./Labs/ee3420lab2/Keypad.h)

### Lab 3: General-Purpose Input-Output (GPIO) – Stepper Motor Control

- **Description**: Control a four-phase unipolar stepper motor using GPIO pins and a ULN2003A/ULN2803A driver.
- **Files**:
  - [Lab 3 Code](./Labs/ee3420lab3/ee3420lab3.ino)
  - [Aux Board Header](./Labs/ee3420lab3/ee3420_mega_aux_board.h)

### Lab 4: General-Purpose Input-Output (GPIO) – Traffic Control

- **Description**: Simulate a traffic light controller for an intersection using two microprocessors communicating via Serial1.
- **Files**:
  - [Lab 4 Code](./Labs/ee3420lab4/ee3420lab4.ino)
  - [Aux Board Header](./Labs/ee3420lab4/ee3420_mega_aux_board.h)

### Lab 5: Pulse-Width Modulation – DC Motor Control and Servo Motor Control

- **Description**: Control a DC motor and a servo motor using PWM signals. Adjust motor speed and servo position based on user inputs.
- **Files**:
  - [Lab 5 Code](./Labs/ee3420lab5/ee3420lab5.ino)
  - [Lab 5 Part 1 Code](./Labs/ee3420lab5p1/ee3420lab5p1.ino)

### Lab 6: Analog-to-Digital Converters (ADC) – Sensors and Measurement

- **Description**: Read sensor data using ADC and control physical systems based on the sensor input. Sensors include a joystick, photoresistor, and thermometer.
- **Files**:
  - [Lab 6 Code](./Labs/ee3420lab6/ee3420lab6.ino)
  - [ADC Board Header](./Labs/ee3420lab6/ee3420_mega_adc_board.h)
  - [Aux Board Header](./Labs/ee3420lab6/ee3420_mega_aux_board.h)

### Lab 7: Serial Peripheral Interconnect (SPI) – Extending GPIO

- **Description**: Use SPI to interact with peripherals, conserving GPIO pins. Interfaces include a 74HC595 shift register and an 8x8 LED dot matrix display.
- **Files**:
  - [Lab 7 Code](./Labs/ee3420lab7/ee3420lab7.ino)
  - [ASCII Header](./Labs/ee3420lab7/ASCII.h)
  - [ADC Board Header](./Labs/ee3420lab7/ee3420_mega_adc_board.h)
  - [Aux Board Header](./Labs/ee3420lab7/ee3420_mega_aux_board.h)

### Lab 8: Inter-Integrated Circuit (I2C) – Real-Time Clock and EEPROM

- **Description**: Interface with a DS3231 RTC and an AT24C32 EEPROM using I2C. Read, set, and store time data.
- **Files**:
  - [Lab 8 Code](./Labs/ee3420lab8/ee3420lab8.ino)
  - [ASCII Header](./Labs/ee3420lab8/ASCII.h)
  - [Aux Board Header](./Labs/ee3420lab8/ee3420_mega_aux_board.h)
  - [RTC Library](./Labs/ee3420lab8/data/RTClib-2.1.1.zip)

### Lab 9: Integrated Application – Weather Station

- **Description**: Create a weather station that gathers and stores weather-related data such as temperature, light level, and air quality.
- **Files**:
  - [Lab 9 Code](./Labs/ee3420lab9/ee3420lab9.ino)
  - [Lab 9 Receiver Code](./Labs/ee3420lab9unoRecieve/ee3420lab9unoRecieve.ino)
  - [ASCII Header](./Labs/ee3420lab9/ASCII.h)
  - [ADC Board Header](./Labs/ee3420lab9/ee3420_mega_adc_board.h)
  - [Aux Board Header](./Labs/ee3420lab9/ee3420_mega_aux_board.h)

## Usage

To run any of the labs, navigate to the respective directory and upload the `.ino` file to your Arduino Mega 2560 using the Arduino IDE. Ensure you have all necessary libraries and headers included.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
