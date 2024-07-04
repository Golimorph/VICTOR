//created by Richard Edberg 2024

#pragma once

#define UART_MESSAGE_QUEUE_SIZE 20
#include <arduinoIf.h>

namespace uart
{

/*! @briefQueue with uart messages from raspberrypi
 * maximum 20 queued tasks at the same time. (seems to be size limitation, will not compile at 256, 
 * can find optimum largest value later if needed.)*/
Deque<std::vector<uint8_t>> uartMessageQueue(UART_MESSAGE_QUEUE_SIZE);


/*! @brief Check the UART messages that has come in to the arduino 
 if a message is in the inbox, then check the message size and loop
 .read() until the full message is fetched. Then put the full message in 
 the uartMessageQueue for processing. */
void checkInbox()
{
	if (Serial.available() > 0) 
	{
		std::vector<uint8_t> uartMessage;
		uint8_t messageType = Serial.read();
		uartMessage.push_back(messageType);

		int messageSize = 0;
		switch(messageType)
		{
			case static_cast<uint8_t>(arduinoIf::arduinoMessageType::MOVE_TRACKS_MESSAGE):
				messageSize = SIZE_MOVE_TRACKS_MESSAGE;
				break;
			case static_cast<uint8_t>(arduinoIf::arduinoMessageType::MOVE_ARM_MESSAGE):
				messageSize = SIZE_MOVE_ARM_MESSAGE;
				break;
			case static_cast<uint8_t>(arduinoIf::arduinoMessageType::MOVE_CLAW_MESSAGE):
				messageSize = SIZE_MOVE_CLAW_MESSAGE;
				break;
			case static_cast<uint8_t>(arduinoIf::arduinoMessageType::MOVE_CLAW_ANGLE_MESSAGE):
				messageSize = SIZE_MOVE_CLAW_ANGLE_MESSAGE;
				break;
			default:
				Serial.println("MessageHandler/uart.h: unknown message received by Arduino.");
		} 

		while(uartMessage.size() < messageSize)
		{
			if (Serial.available() > 0) 
			{	
				uartMessage.push_back(Serial.read());
			}
		}

		uartMessageQueue.push_back(uartMessage);
		if(uartMessageQueue.count() >= UART_MESSAGE_QUEUE_SIZE)
		{
			Serial.println("WARNING: UART Request queue overflow! Request dropped!");
		}
	}
}
}