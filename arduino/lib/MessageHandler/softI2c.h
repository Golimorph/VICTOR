#ifndef SOFTI2C_H
#define SOFTI2C_H

#include <Arduino.h>
#include <SoftWire.h>

class SoftI2c
{
public:
    SoftI2c(uint8_t sdaPin, uint8_t sclPin);
    void begin();
    void checkForData();

private:
    SoftWire wire;
    const uint8_t i2cAddress = 0x09;
    void receiveEvent(int bytes);
};

#endif