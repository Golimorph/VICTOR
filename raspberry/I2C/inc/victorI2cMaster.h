//created by Richard Edberg 2024
/*! @brief This class handles I2C communication between the Raspberrypi5 and the Arduino Uno 
* inside victor. When constructed it initializes the I2C connection on the Raspberry side.
* the send method can then be used to send messages to victor. The messages consist of 
* on-byte I2C numbers sent in sequence. The first byte indicates which message is being sent.
* The arduinoIf.h has a list of the number of bytes (size) of each message. The following
* bytes after the message indicator byte will be sent directly after, and the Arduino (I2C slave)
* will collect the message on the other side of this communication. */ 

#pragma once

#include <string>
#include <wiringPiI2C.h>
#include "arduinoIf.h"
#include <iostream>
#include <map>
#include <vector>

class VictorI2cMaster
{
public: 
	VictorI2cMaster();
	/*! @brief the send message takes a string which is on kotlin data-class format 
	 * and converts it into a series of bytes (first byte denotes which signal and size of message in number of bytes) 
	 * and the following bytes represent the information in the message. Typically one byte per field of the signal. 
	 * It breakes down the message into bytes using the message specification in arduinoIf.h*/
	bool send(std::string message);

private:
	/*! @brief Initializes the I2C communication with Victor arduino 
	 * so that the send() method can be used. It is called from the constructor.*/
	bool init();
	bool isValid(arduinoIf::arduinoMessageType arduinoMessage, std::vector<uint8_t> totalMessage);
	arduinoIf::arduinoMessageType extractMessageName(const std::string& input);//helper method to get message type from kotlin string
	std::vector<uint8_t> extractMessageData(const std::string& input);
	int m_fd; //The identifier for the I2C link. Used when writing. Initialized in init()
	static constexpr uint8_t DEVICE_ID = 0x09;
	std::map<std::string, arduinoIf::arduinoMessageType> m_messageStringEnumMap;//The caller provides a string for the message, but in I2C an enum is used as it is less bug prone.
	std::map<arduinoIf::arduinoMessageType, uint8_t> m_arduinoMessageSizeMap;//The number of bytes to be sent over I2C for each message.
};
