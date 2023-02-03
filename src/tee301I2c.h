/***************************************************************************/
/* headerfile for "tee301I2c.cpp" module */
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

#ifndef tee301I2c_H
#define tee301I2c_H

#include "Arduino.h"
#include "Wire.h"

// Defines
//-----------------------------------------------------------------------------
#define CRC8_ONEWIRE_POLY                                       0x31
#define CRC8_ONEWIRE_START                                      0xFF
#define TEE301_COMMAND_READ_SINGLE_SHOT_HIGH_EN                 0x2C06 //EN = clock stretching enabled
#define TEE301_COMMAND_READ_SINGLE_SHOT_MEDIUM_EN               0x2C0D //EN = clock stretching enabled
#define TEE301_COMMAND_READ_SINGLE_SHOT_LOW_EN                  0x2C10 //EN = clock stretching enabled
#define TEE301_COMMAND_READ_SINGLE_SHOT_HIGH_DIS                0x2C00 //DIS = clock stretching disabled
#define TEE301_COMMAND_READ_SINGLE_SHOT_MEDIUM_DIS              0x2C0B //DIS = clock stretching disabled
#define TEE301_COMMAND_READ_SINGLE_SHOT_LOW_DIS                 0x2C16 //DIS = clock stretching disabled 
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_0_5_HIGH      0x2032
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_0_5_MEDIUM    0x2024
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_0_5_LOW       0x202F
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_1_HIGH        0x2130
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_1_MEDIUM      0x2126
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_1_LOW         0x212D
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_2_HIGH        0x2236
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_2_MEDIUM      0x2220
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_2_LOW         0x222B
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_4_HIGH        0x2334
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_4_MEDIUM      0x2322
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_4_LOW         0x2329
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_10_HIGH       0x2737
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_10_MEDIUM     0x2721
#define TEE301_COMMAND_START_PERIODIC_MEASUREMENT_10_LOW        0x272A
#define TEE301_COMMAND_READ_PERIODIC_MEASUREMENT                0xE000
#define TEE301_COMMAND_CLEAR_REGISTER                           0x3041
#define TEE301_COMMAND_READ_REGISTER                            0xF32D
#define TEE301_COMMAND_END_PERIODIC_MEASUREMENT                 0x3093
#define TEE301_COMMAND_SOFT_RESET                               0x30A2
#define TEE301_COMMAND_HEATER_ON                                0x306D
#define TEE301_COMMAND_HEATER_OFF                               0x3066
#define TEE301_COMMAND_READ_IDENTIFICATION                      0x7029


// declaration of functions
//-----------------------------------------------------------------------------

class tee301I2c
{
public:
    tee301I2c(void);
    tee301I2c(unsigned char i2cAdress);
    uint8_t singleShotTemp(float &temperature, int repeatability); // with clock stretching enabled
    uint8_t singleShotTempClockStretchingDisabled(float &temperature, int repeatability); // with clock stretching disabled
    uint8_t getPeriodicMeasurementTemp(float &temperature); 
    void startPeriodicMeasurement(int measurementPerSeconds , int repeatability);
    void endPeriodicMeasurement(void);
    void heaterOn(void);
    void heaterOff(void);
    uint8_t readIdentification(unsigned char identification[]);
    uint8_t constantHeaterOnOff(bool &conHeaterOnOff);
    uint8_t readStatusRegister(unsigned char statusRegister[]);
    void clearStatusregister(void);
    void reset(void);
    void i2cReset(void);
    unsigned char address = 0x4A;
    void wireWrite(unsigned char buf[], int to, bool stopmessage);
    void wireRead(unsigned char buf[], uint8_t to);
    unsigned char calcCrc8(unsigned char buf[], unsigned char from, unsigned char to);
    void getErrorString(uint8_t Status, char errorString[]);
};

#endif