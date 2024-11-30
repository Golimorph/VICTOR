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
    moveArmMessage.a = static_cast<int8_t>(message[1]);
    moveArmMessage.b = static_cast<int8_t>(message[2]);
    moveArmMessage.c = static_cast<int8_t>(message[3]);
    moveArmMessage.d = static_cast<int8_t>(message[4]);
    moveArmMessage.e = static_cast<int8_t>(message[5]);
    moveArmMessage.f = static_cast<int8_t>(message[6]);

    const int t = 0; //Time for move [ms].
    return m_sf->moveArm(moveArmMessage.a, moveArmMessage.b, moveArmMessage.c, moveArmMessage.d, moveArmMessage.e, moveArmMessage.f, t);
}

bool MessageHandler::handleMoveClawMessage(std::vector<uint8_t> message)
{
    arduinoIf::MoveClawMessage moveClawMessage;
    moveClawMessage.state = static_cast<int8_t>(message[1]);

    const int t = 0; //Time for move [ms].
    if(moveClawMessage.state == 1)
    {
        m_sf->moveServo(CLAWSERVO, -54, t);
    }
    else
    {
        m_sf->moveServo(CLAWSERVO, 40, t);
    }
    return true;
}