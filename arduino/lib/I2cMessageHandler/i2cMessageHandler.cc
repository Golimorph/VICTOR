//created by Richard Edberg 2024
#include "i2cMessageHandler.h"

I2cMessageHandler::I2cMessageHandler(ServoFunctions& sf)
{
	m_sf = &sf; 
}

bool I2cMessageHandler::handleMessage(std::vector<uint8_t> message)
{
	if(message.size() < 1)
	{
		return false;
	}

	switch(message[0])
	{
		case static_cast<uint8_t>(arduinoIf::arduinoMessageType::NO_MESSAGE):
			return true;
		case static_cast<uint8_t>(arduinoIf::arduinoMessageType::MOVE_TRACKS_MESSAGE):
			return handleMoveTracksMessage(message);
		default:
			return false;
	} 
	return false;
}

bool I2cMessageHandler::handleMoveTracksMessage(std::vector<uint8_t> message)
{
	arduinoIf::MoveTracksMessage messageStruct;
	messageStruct.leftTrackSpeed = static_cast<int8_t>(message[1]);
	messageStruct.rightTrackSpeed = static_cast<int8_t>(message[2]);
	m_sf->setMotorSpeed(messageStruct.leftTrackSpeed,messageStruct.rightTrackSpeed);
	return true; 
}

