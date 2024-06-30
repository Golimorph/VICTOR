#include "softI2c.h"

SoftI2c::SoftI2c(uint8_t sdaPin, uint8_t sclPin)
    : wire(sdaPin, sclPin) {}

void SoftI2c::begin() 
{
    // SoftWire doesn't support slave mode directly, so this is just an initialization
    wire.begin();
    Serial.begin(9600);
    Serial.println("SoftWire I2C Initialized");
}

void SoftI2c::checkForData() 
{
    // Poll for incoming I2C data
    if (wire.available()) 
    {
        receiveEvent(wire.available());
    }
}

void SoftI2c::receiveEvent(int bytes) 
{
    while (bytes-- > 0) 
    {
        char c = wire.read();
        Serial.print("Received: ");
        Serial.println(c);
    }
}
