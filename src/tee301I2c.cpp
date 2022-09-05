/***************************************************************************/
/* sourcefile for "tee301I2c.h" module */
/***************************************************************************/
/*
Read functions for measurement values of the TEE301 Sensor via I2C interface.

Copyright 2022 E+E Elektronik Ges.m.b.H.

Disclaimer:
This application example is non-binding and does not claim to be complete with regard
to configuration and equipment as well as all eventualities. The application example
is intended to provide assistance with the TEE301 sensor module design-in and is provided "as is".
You yourself are responsible for the proper operation of the products described.
This application example does not release you from the obligation to handle the product safely
during application, installation, operation and maintenance. By using this application example,
you acknowledge that we cannot be held liable for any damage beyond the liability regulations
described.

We reserve the right to make changes to this application example at any time without notice.
In case of discrepancies between the suggestions in this application example and other E+E
publications, such as catalogues, the content of the other documentation takes precedence.
We assume no liability for the information contained in this document.
*/

// Includes
//-----------------------------------------------------------------------------
#include "tee301I2c.h"
#include <Arduino.h>
#include "Wire.h"


enum Errorcode
{
    OKAY = 0,
    ERR_CKSUM = 1
};


tee301I2c::tee301I2c(void)
{
}

tee301I2c::tee301I2c(unsigned char i2cAdress)
{
  address = i2cAdress;
}

uint8_t tee301I2c::singleShotTemp(float &temperature, int repeatability) // repeatability: 0 = low, 1 = medium, 2 = high;
{
  unsigned char i2cResponse[6];
  unsigned char Command[] = {0x2C, 0x00};
  switch (repeatability)
  {
  case 0:
    Command[1] = {0x10};
    break;
  case 1:
    Command[1] = {0x0D};
    break;
  case 2:
    Command[1] = {0x06};
    break;
  }
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    temperature = -45 + 175 * ((float)(i2cResponse[0]) * 256 + i2cResponse[1]) / 65535;
    return 0;
  }
  else
  {
    return 1;
  }
}


uint8_t tee301I2c::singleShotTempClockStretchingDisabled(float &temperature, int repeatability) // repeatability: 0 = low, 1 = medium, 2 = high;
{
  unsigned char i2cResponse[6];
  unsigned char Command[] = {0x24, 0x00};
  switch (repeatability)
  {
  case 0:
    Command[1] = {0x16};
    break;
  case 1:
    Command[1] = {0x0B};
    break;
  case 2:
    Command[1] = {0x00};
    break;
  }
  wireWrite(Command, 1, false);
  delay(15);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    temperature = -45 + 175 * ((float)(i2cResponse[0]) * 256 + i2cResponse[1]) / 65535;
    return 0;
  }
  else
  {
    return 1;
  }
}


uint8_t tee301I2c::getPeriodicMeasurementTemp(float &temperature)
{
  unsigned char i2cResponse[6];
  unsigned char Command[] = {0xE0, 0x00};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 6);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    temperature = -45 + 175 * ((float)(i2cResponse[0]) * 256 + i2cResponse[1]) / 65535;
    return 0;
  }
  else
  {
    return 1;
  }
}


void tee301I2c::startPeriodicMeasurement(int measurementPerSeconds , int repeatability) // measurementPerSeconds: 0 = 0.5 mps, 1 = 1mps, 2 = 2mps, 3 = 4mps, 4 = 10mps;  repeatability: 0 = low, 1 = medium, 2 = high; 
{
  unsigned char i2cResponse[6];
  unsigned char Command[] = {0x00, 0x00};
  switch (measurementPerSeconds)
  {
  case 0:
    Command[0] = {0x20};
    switch (repeatability)
    {
      case 0:
        Command[1] = {0x2F};
        break;
      case 1:
        Command[1] = {0x24};
        break;
      case 2:
        Command[1] = {0x32};
        break;
    }
    break;
  case 1:
    Command[0] = {0x21};
    switch (repeatability)
    {
      case 0:
        Command[1] = {0x2D};
        break;
      case 1:
        Command[1] = {0x26};
        break;
      case 2:
        Command[1] = {0x30};
        break;
    }
    break;
  case 2:
    Command[0] = {0x22};
    switch (repeatability)
    {
      case 0:
        Command[1] = {0x2B};
        break;
      case 1:
        Command[1] = {0x20};
        break;
      case 2:
        Command[1] = {0x36};
        break;
    }
    break;
  case 3:
    Command[0] = {0x23};
    switch (repeatability)
    {
      case 0:
        Command[1] = {0x29};
        break;
      case 1:
        Command[1] = {0x22};
        break;
      case 2:
        Command[1] = {0x34};
        break;
    }
    break;
  case 4:
    Command[0] = {0x27};
    switch (repeatability)
    {
      case 0:
        Command[1] = {0x2A};
        break;
      case 1:
        Command[1] = {0x21};
        break;
      case 2:
        Command[1] = {0x37};
        break;
    }
    break;
  }
  wireWrite(Command, 1, false);
}

void tee301I2c::endPeriodicMeasurement(void)
{
  unsigned char Command[] = {0x30, 0x93};
  wireWrite(Command, 1, true);
}

void tee301I2c::heaterOn(void)
{
  unsigned char Command[] = {0x30, 0x6D};
  wireWrite(Command, 1, true);
}

void tee301I2c::heaterOff(void)
{
  unsigned char Command[] = {0x30, 0x66};
  wireWrite(Command, 1, true);
}

uint8_t tee301I2c::readIdentification(unsigned char identification[])
{
  unsigned char i2cResponse[9];
  unsigned char Command[] = {0x70, 0x29};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 9);
  if (i2cResponse[8] == calcCrc8(i2cResponse, 0, 7))
  {
    for (int i = 0; i < 8; i++)
    {
      identification[i] = i2cResponse[i];
    }
    return 0;
  }
  else
  {
    return 1;
  }
}


uint8_t tee301I2c::readStatusRegister1(unsigned char statusRegister1[])
{
  unsigned char i2cResponse[3];
  unsigned char Command[] = {0xF3, 0x2D};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    for (int i = 0; i < 2; i++)
    {
      statusRegister1[i] = i2cResponse[i];
    }
    return 0;
  }
  else
  {
    return 1;
  }
}


uint8_t tee301I2c::readStatusRegister2(unsigned char statusRegister2[])
{
  unsigned char i2cResponse[3];
  unsigned char Command[] = {0xF3, 0x2D};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    for (int i = 0; i < 2; i++)
    {
      statusRegister2[i] = i2cResponse[i];
    }
    return 0;
  }
  else
  {
    return 1;
  }
}


void tee301I2c::reset(void)
{
  unsigned char Command[] = {0x30, 0xA2};
  wireWrite(Command, 1, true);
}


uint8_t tee301I2c::constantHeaterOnOff(bool &conHeaterOnOff)
{
  unsigned char i2cResponse[3];
  unsigned char Command[] = {0xF3, 0x2D};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    i2cResponse[0] = i2cResponse[0] << 2;
    conHeaterOnOff = i2cResponse[0] >> 7;
    return 0;
  }
  else
  {
    return 1;
  }
}

void tee301I2c::clearStatusregister1(void)
{
  unsigned char Command[] = {0x30, 0x41};
  wireWrite(Command, 1, true);
}


void tee301I2c::i2cReset(void)
{
  Wire.beginTransmission(0x00);
  Wire.write(0x06);
  Wire.endTransmission(true);
}


void tee301I2c::wireWrite(unsigned char buf[], int to, bool stopmessage)
{
  Wire.beginTransmission(address);
  for (int i = 0; i <= to; i++)
  {
    Wire.write(buf[i]);
  }
  Wire.endTransmission(stopmessage);
}

void tee301I2c::wireRead(unsigned char buf[], uint8_t to)
{
  int i = 0;
  Wire.requestFrom(address, to);
  while (Wire.available())
  {
    buf[i++] = Wire.read();
  }
}

unsigned char tee301I2c::calcCrc8(unsigned char buf[], unsigned char from, unsigned char to)
{
  unsigned char crcVal = CRC8_ONEWIRE_START;
  unsigned char i = 0;
  unsigned char j = 0;
  for (i = from; i <= to; i++)
  {
    int curVal = buf[i];
    for (j = 0; j < 8; j++)
    {
      if (((crcVal ^ curVal) & 0x80) != 0) //If MSBs are not equal
      {
        crcVal = ((crcVal << 1) ^ CRC8_ONEWIRE_POLY);
      }
      else
      {
        crcVal = (crcVal << 1);
      }
      curVal = curVal << 1;
    }
  }
  return crcVal;
}

void tee301I2c::getErrorString(uint8_t Status, char errorString[])
{
  memset(errorString, '\0', sizeof(errorString));
  switch (Status)
  {
  case OKAY:
    strcpy(errorString, "Success");
    break;
  case ERR_CKSUM:
    strcpy(errorString, "Checksum error");
    break;
  default:
    strcpy(errorString, "unknown error");
    break;
  }
}