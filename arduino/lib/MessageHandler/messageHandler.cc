//created by Richard Edberg 2024
#include "messageHandler.h"

MessageHandler::MessageHandler(ServoFunctions& sf)
{
	m_sf = &sf; 
}

bool MessageHandler::handleMessage(std::vector<uint8_t> message)
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
		case static_cast<uint8_t>(arduinoIf::arduinoMessageType::MOVE_ARM_MESSAGE):
			return handleMoveArmMessage(message);
		case static_cast<uint8_t>(arduinoIf::arduinoMessageType::MOVE_CLAW_MESSAGE):
			return handleMoveClawMessage(message);
		case static_cast<uint8_t>(arduinoIf::arduinoMessageType::MOVE_CLAW_ANGLE_MESSAGE):
			return handleMoveClawAngleMessage(message);
		default:
			return false;
	} 
	return false;
}

bool MessageHandler::handleMoveTracksMessage(std::vector<uint8_t> message)
{
	arduinoIf::MoveTracksMessage messageStruct;
	messageStruct.leftTrackSpeed = static_cast<int8_t>(message[1]);
	messageStruct.rightTrackSpeed = static_cast<int8_t>(message[2]);
	m_sf->setMotorSpeed(messageStruct.leftTrackSpeed,messageStruct.rightTrackSpeed);
	return true; 
}


bool MessageHandler::handleMoveArmMessage(std::vector<uint8_t> message)
{
	arduinoIf::MoveArmMessage moveArmMessage;
	moveArmMessage.xcm = static_cast<int8_t>(message[1]);
	moveArmMessage.xmm = static_cast<int8_t>(message[2]);
	moveArmMessage.ycm = static_cast<int8_t>(message[3]);
	moveArmMessage.ymm = static_cast<int8_t>(message[4]);
	moveArmMessage.zcm = static_cast<int8_t>(message[5]);
	moveArmMessage.zmm = static_cast<int8_t>(message[6]);

	double x = 10*moveArmMessage.xcm + moveArmMessage.xmm;//mm
	double y = 10*moveArmMessage.ycm + moveArmMessage.ymm;//mm
	double z = 10*moveArmMessage.zcm + moveArmMessage.zmm;//mm
	
	const int t = 10; //Time for move [ms].
	return m_sf->moveArm(x, y, z, t); 
}

bool MessageHandler::handleMoveClawMessage(std::vector<uint8_t> message)
{
	arduinoIf::MoveClawMessage moveClawMessage;
	moveClawMessage.state = static_cast<int8_t>(message[1]);

	const int t = 0; //Time for move [ms].
	if(moveClawMessage.state == 1)
	{
		m_sf->moveServo(CLAWSERVO, -54, t);
	}else
	{
		m_sf->moveServo(CLAWSERVO, 40, t);
	}
	return true;
}

bool MessageHandler::handleMoveClawAngleMessage(std::vector<uint8_t> message)
{
	arduinoIf::MoveClawAngleMessage moveClawAngleMessage;
	moveClawAngleMessage.x = static_cast<int8_t>(message[1]);
	moveClawAngleMessage.y = static_cast<int8_t>(message[2]);
	moveClawAngleMessage.z = static_cast<int8_t>(message[3]);

	//create a unit vector from the message.
	double x = static_cast<double>(moveClawAngleMessage.x);
	double y = static_cast<double>(moveClawAngleMessage.y);
	double z = static_cast<double>(moveClawAngleMessage.z);
	if(moveClawAngleMessage.x == 0 && moveClawAngleMessage.y == 0 && moveClawAngleMessage.z == 0)
	{
		x = 0;
		y = 1;
		z = 0;
	}else
	{
		double norm = sqrt(x*x+y*y+z*z);
		x = x/norm;
		y = y/norm;
		z = z/norm;
	}
	const int t = 0; //Time for move [ms].
	m_sf->moveClawAngle(x, y, z, t);

	return true;
}













