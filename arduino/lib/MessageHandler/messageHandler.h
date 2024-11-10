//created by Richard Edberg 2024
#pragma once

#include <arduinoIf.h>
#include <ServoFunctions.h>

class MessageHandler
{
public:
    MessageHandler(ServoFunctions& sf);

    bool handleMessage(std::vector<uint8_t> message);
private:
    bool handleMoveTracksMessage(std::vector<uint8_t> message);
    bool handleMoveArmMessage(std::vector<uint8_t> message);
    bool handleMoveClawMessage(std::vector<uint8_t> message);
    ServoFunctions* m_sf;
};



