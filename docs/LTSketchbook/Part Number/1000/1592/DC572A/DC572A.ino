/*!
Linear Technology DC572A Demonstration Board.
LTC1592: 16-bit SoftSpan DAC with Programmable Output Range

@verbatim
NOTES
  Setup:
   Set the terminal baud rate to 115200 and select the newline terminator.

   An external +/- 15V power supply is required to power the circuit.

  Explanation of Commands:
   1- Voltage Output
   Displays the calculated voltage depending on the code input from user and
   voltage range selected.

   2- Square Wave Output
   Generates a square wave on the output pin. This function helps to measure
   settling time and glitch impulse.

   3- Change Range
   |   Command   | Range Selected |
   | C3 C2 C1 C0 |                |
   |------------------------------|
   | 1  0  0  0  |    0V - 5V     |
   | 1  0  0  1  |    0V - 10V    |
   | 1  0  1  0  |   -5V - +5V    |
   | 1  0  1  1  |  -10V - +10V   |
   | 1  1  0  0  | -2.5V - +2.5V  |
   | 1  1  0  1  | -2.5V - 7V     |


USER INPUT DATA FORMAT:
 decimal : 1024
 hex     : 0x400
 octal   : 02000  (leading 0 "zero")
 binary  : B10000000000
 float   : 1024.0

@endverbatim

http://www.linear.com/product/LTC1592

http://www.linear.com/product/LTC1592#demoboards

REVISION HISTORY
$Revision: 3238 $
$Date: 2015-03-06 16:40:13 -0800 (Fri, 06 Mar 2015) $

Copyright (c) 2013, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However, the Linduino is only possible because of the Arduino team's commitment
to the open-source community.  Please, visit http://www.arduino.cc and
http://store.arduino.cc , and consider a purchase that will help fund their
ongoing work.
*/

/*! @file
    @ingroup LTC1592
*/

// Headerfiles
#include "LT_SPI.h"
#include "UserInterface.h"
#include "LT_I2C.h"
#include "QuikEval_EEPROM.h"
#include "Linduino.h"
#include <SPI.h>
#include "LTC1592.h"
#include <QVariant>   // for parseFloat()
#include <Stream.h>

// Global variables
static uint8_t demo_board_connected;   //!< Set to 1 if the board is connected
float RANGE_LOW = 0;
float RANGE_HIGH = 5;
uint16_t data = 0x0000;
uint8_t choice = 0x00;

// Function Declarations
void print_title();
void print_prompt();
uint8_t voltage_output();
uint8_t square_wave_output();
void change_range();

//! Initialize Linduino
void setup()
{
  char demo_name[] = "DC572";  // Demo Board Name stored in QuikEval EEPROM

  quikeval_SPI_init();          // Configure the spi port for 4MHz SCK
  quikeval_SPI_connect();       // Connect SPI to main data port
  quikeval_I2C_init();          // Configure the EEPROM I2C port for 100kHz

  Serial.begin(115200);         // Initialize the serial port to the PC
  print_title();

  read_quikeval_id_string(&ui_buffer[0]);
  ui_buffer[48] = 0;
  Serial.println(ui_buffer);

  demo_board_connected = discover_demo_board(demo_name);
  if (demo_board_connected)
  {
    Serial.println("Connected...");
    print_prompt();
  }
}

//! Repeats Linduino loop
void loop()
{
  int16_t user_command;
  if (Serial.available())               // Check for user input
  {
    user_command = read_int();          // Read the user command
    Serial.println(user_command);
    Serial.flush();
    switch (user_command)
    {
      case 1:
        voltage_output();
        break;
      case 2:
        square_wave_output();
        break;
      case 3:
        change_range();
        break;
      default:
        Serial.println(F("Incorrect Option"));
        break;
    }
    Serial.println(F("\n************************************************************"));
    print_prompt();
  }
}

//! Prints the title block when program first starts.
void print_title()
{
  Serial.println();
  Serial.println(F("*****************************************************************"));
  Serial.println(F("* DC572A Demonstration Program                                  *"));
  Serial.println(F("*                                                               *"));
  Serial.println(F("* This program demonstrates how to send data to the LTC1592     *"));
  Serial.println(F("* 16-bit SoftSpan DAC found on the DC572A demo board.           *"));
  Serial.println(F("*                                                               *"));
  Serial.println(F("* Set the baud rate to 115200 and select the newline terminator.*"));
  Serial.println(F("*                                                               *"));
  Serial.println(F("*****************************************************************\n"));
}

//! Prints main menu.
void print_prompt()
{
  Serial.println(F("\nCommand Summary:"));
  Serial.println(F("  1-Voltage Output"));
  Serial.println(F("  2-Square wave output"));
  Serial.println(F("  3-Change Range"));

  Serial.println(F("\nPresent Values:"));
  Serial.print(F("  Range: "));
  switch (choice)
  {
    case 0:
      Serial.print(F("0V to 5V"));
      break;

    case 1:
      Serial.print(F("0V to 10V"));
      break;

    case 2:
      Serial.print(F("-5V to +5V"));
      break;

    case 3:
      Serial.print(F("-10V to +10V"));
      break;

    case 4:
      Serial.print(F("-2.5V to +2.5V"));
      break;

    case 5:
      Serial.print(F("-2.5V to 7V"));
      break;

    default:
      Serial.println("0V to 5V");
  }

  Serial.print(F("\n\nEnter a command: "));
  Serial.flush();
}

//! Function to enter a digital value and get the analog output
uint8_t voltage_output()
{
  float voltage;
  Serial.print("\nEnter the 16-bit data as decimal or hex: ");
  data = read_int();
  Serial.print("0x");
  Serial.println(data, HEX);

  LTC1592_write(LTC1592_CS, data, choice);
  voltage = LTC1592_code_to_voltage(data, RANGE_HIGH, RANGE_LOW);

  Serial.print("\nOutput voltage = ");
  Serial.print(voltage);
  Serial.println(" V");
  return 0;
}

//! Function to generate a square wave of desired frequency and voltage ranges
uint8_t square_wave_output()
{
  uint16_t freq;
  float time;
  float voltage_high, voltage_low;
  uint16_t code_high, code_low;
  uint8_t receive_enter;  // To receive enter key pressed

  Serial.print("\nEnter voltage_high: ");
  while (!Serial.available());
  voltage_high = Serial.parseFloat();
  Serial.print(voltage_high);
  Serial.println(" V");
  receive_enter = read_int();

  Serial.print("\nEnter voltage_low: ");
  while (!Serial.available());
  voltage_low = Serial.parseFloat();
  Serial.print(voltage_low);
  Serial.println(" V");
  receive_enter = read_int();

  Serial.print("\nEnter the required frequency in Hz: ");
  freq = read_int();
  Serial.print(freq);
  Serial.println(" Hz");

  time = (float)1000/freq;
  Serial.print("\nT = ");
  Serial.print(time);
  Serial.println(" ms");

  //! Converting data into voltage
  code_high = LTC1592_voltage_to_code(voltage_high, RANGE_HIGH, RANGE_LOW);
  code_low = LTC1592_voltage_to_code(voltage_low, RANGE_HIGH, RANGE_LOW);

  while (!Serial.available()) //! Generate square wave until a key is pressed
  {
    LTC1592_write(LTC1592_CS, code_high, choice);
    delayMicroseconds(time * 500);
    LTC1592_write(LTC1592_CS, code_low, choice);
    delayMicroseconds(time * 500);
  }
  receive_enter = read_int();
  return 0;
}

//! Function to choose the range of voltages to be used
void change_range()
{
  Serial.println("\n| Choice | Range         |");
  Serial.println("|--------|---------------|");
  Serial.println("|    0   | 0 - 5 V       |");
  Serial.println("|    1   | 0 - 10 V      |");
  Serial.println("|    2   | -5 - +5 V     |");
  Serial.println("|    3   | -10 - +10 V   |");
  Serial.println("|    4   | -2.5 - +2.5 V |");
  Serial.println("|    5   | -2.5 - 7 V    |");

  Serial.print("\nEnter your choice: ");
  choice = read_int();
  Serial.println(choice);

  switch (choice)
  {
    case 0:
      RANGE_LOW = 0;
      RANGE_HIGH = 5;
      break;

    case 1:
      RANGE_LOW = 0;
      RANGE_HIGH = 10;
      break;

    case 2:
      RANGE_LOW = -5;
      RANGE_HIGH = 5;
      break;

    case 3:
      RANGE_LOW = -10;
      RANGE_HIGH = 10;
      break;

    case 4:
      RANGE_LOW = -2.5;
      RANGE_HIGH = 2.5;
      break;

    case 5:
      RANGE_LOW = -2.5;
      RANGE_HIGH = 7;
      break;

    default:
      Serial.println("\nWrong choice!");
  }
}
