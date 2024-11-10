//created by Richard Edberg 2024

#pragma once

#include <Wire.h>

#define I2C_MESSAGE_QUEUE_SIZE 20

namespace i2c
{

/*! @briefQueue with I2C messages from raspberrypi
 * maximum 20 queued tasks at the same time. (seems to be size limitation, will not compile at 256,
 * can find optimum largest value later if needed.)*/
Deque<std::vector<uint8_t>> i2cMessageQueue(I2C_MESSAGE_QUEUE_SIZE);


/*! @brief All I2C requests are put in a queue and exectued in order. When execution is completed
 *they will be popped from the the queue. An emtpy queue means nothing will happen.
 *Every new I2C message is pushed to the back of the queue. This is done in the receiveEvent
 *method wich is a callback method called everytime a new I2C message is received.*/
void onReceiveI2cCallback(int bytes)
{
    std::vector<uint8_t> i2cMessage;
    for(uint8_t byte = 0; byte < bytes; ++byte)
    {
        i2cMessage.push_back(Wire.read());
    }
    i2cMessageQueue.push_back(i2cMessage);

    if(i2cMessageQueue.count() >= I2C_MESSAGE_QUEUE_SIZE)
    {
        Serial.println("ERROR! Request queue overflow! Request dropped!");
    }
}

/*! @brief Arduino Wire.h initializes I2C by giving the arduino and address and
 * registering a callback to be called when an I2C message on that address is received*/
void initI2C()
{
    Wire.begin(9); //I2C slave
    Wire.onReceive(onReceiveI2cCallback);// I2C slave
}


}