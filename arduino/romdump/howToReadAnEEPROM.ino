/*
  Reading I2C EEPROMs: Large and Small
  SparkFun Electronics
  Date: June 11th, 2017

  Demonstrate how to read the entirety of an EEPROM and output it to a terminal.
  This sketch shows how to read the entire contents of a 24LC1025 I2C EEPROM.

  You'll need to use a terminal to capture the incoming contents to a text file. The
  Arduino terminal will not work. We recommend Teraterm: 
  https://learn.sparkfun.com/tutorials/terminal-basics
  
  Hardware Connections (IC to Arduino):
  -Pin 1(A0) = GND
  -Pin 2(A1) = GND
  -Pin 3(A2) = 3.3V or 5V
  -Pin 4(VSS) = GND
  -Pin 5(SDA) = SDA
  -Pin 6(SCL) = SCL
  -Pin 7(WP) = GND (No write protection)
  -Pin 8(VCC) = 3.3V or 5V

  Two pull up resistors are needed as well:
  -4.7K SDA = VCC
  -4.7K SCL = VCC

  EEPROMs general have thousands of bytes that will be stored even after power is disconnected.
  There are some EEPROMs that are large enough they require an extra address. The 24LC1025 is
  one example.
*/

#include <Wire.h>

//The 24LC1025 has a weird I2C address. The IC responds to two addresses in practice:
//0b.101.0000 (0x50) and 0b.101.0100 (0x54). There is a block select bit (bit 2) that
//allows us to read the full 17-bit address space
#define EEPROM_ADR_LOW_BLOCK 0x50 //0b.101.0000 0x50
#define EEPROM_ADR_HIGH_BLOCK 0x54 //0b.101.0100 0x54

void setup()
{
  Wire.begin();
  Wire.setClock(400000); //Let's go fast! Set I2C clock to 400kHz

  Serial.begin(115200);

  //Output raw bytes to terminal
  for (long x = 0 ; x < 0x1FFFF ; x++) //Read all 131,071 bytes from EERPOM
  {
    byte val = readEEPROM(x);
    Serial.write(val);
  }
}

void loop()
{
  //Nothing to do, just hang out.
}

//The 24LC1025 is 1024kbit or 128k bytes.
//If the eeaddress is less than the 64k byte threshold we use I2C address 0x50
//If the address is above 65535 then we use 0x54 address
//Based on: https://playground.arduino.cc/Code/I2CEEPROM
void writeEEPROM(long eeAddress, byte data)
{
  if (eeAddress < 65536)
  {
    Wire.beginTransmission(EEPROM_ADR_LOW_BLOCK);
    eeAddress &= 0xFFFF; //Erase the first 16 bits of the long variable
  }
  else
  {
    Wire.beginTransmission(EEPROM_ADR_HIGH_BLOCK);
  }

  Wire.write((int)(eeAddress >> 8)); // MSB
  Wire.write((int)(eeAddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
}

byte readEEPROM(long eeaddress)
{
  if (eeaddress < 65536)
    Wire.beginTransmission(EEPROM_ADR_LOW_BLOCK);
  else
    Wire.beginTransmission(EEPROM_ADR_HIGH_BLOCK);

  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  if (eeaddress < 65536)
    Wire.requestFrom(EEPROM_ADR_LOW_BLOCK, 1);
  else
    Wire.requestFrom(EEPROM_ADR_HIGH_BLOCK, 1);

  byte rdata = 0xFF;
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

//We can read up to 128 bytes at a time
byte readEEPROMPage(long eeaddress)
{
  if (eeaddress < 65536)
    Wire.beginTransmission(EEPROM_ADR_LOW_BLOCK);
  else
    Wire.beginTransmission(EEPROM_ADR_HIGH_BLOCK);

  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  if (eeaddress < 65536)
    Wire.requestFrom(EEPROM_ADR_LOW_BLOCK, 1);
  else
    Wire.requestFrom(EEPROM_ADR_HIGH_BLOCK, 1);

  byte rdata = 0xFF;
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}



