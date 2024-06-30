//created by Richard Edberg 2024
#include "victorI2cMaster.h"



bool VictorI2cMaster::init()
{
    m_fd = wiringPiI2CSetup(DEVICE_ID);
    if (m_fd == -1) {
        std::cerr << "Failed to init I2C communication.\n";
        return false;
    }
    std::cout << "I2C communication successfully setup.\n";
    return true;
}

arduinoIf::arduinoMessageType VictorI2cMaster::extractMessageName(const std::string& input) 
{
    size_t pos = input.find('(');//can assume to always work since string is kotlin-data-class toString output.
    std::string messageName = input.substr(0, pos);


    if(m_messageStringEnumMap.find(messageName) == m_messageStringEnumMap.end())
    {
        return arduinoIf::arduinoMessageType::NO_MESSAGE;    
    }

    return m_messageStringEnumMap.at(messageName);
   
}

std::vector<uint8_t> VictorI2cMaster::extractMessageData(const std::string& input) {
    std::vector<uint8_t> numbers;
    size_t pos = 0;
    while ((pos = input.find('=', pos)) != std::string::npos) 
    {
        pos++;
        uint8_t num = static_cast<uint8_t>(std::stoi(input.substr(pos)));
        size_t numEnd = input.find_first_not_of("-0123456789", pos); //Find the end of the number
        pos = (numEnd == std::string::npos) ? input.length() : numEnd;
        numbers.push_back(num);
    }

    return numbers;
}

bool VictorI2cMaster::isValid(arduinoIf::arduinoMessageType arduinoMessage, std::vector<uint8_t> totalMessage)
{
    if(m_arduinoMessageSizeMap.at(arduinoMessage)!=totalMessage.size())
    {
        std::cerr<<"The message size is invalid, expected " << m_arduinoMessageSizeMap.at(arduinoMessage) << " bytes, " << " but only managed to parse " << totalMessage.size() << "bytes from the string (message type included)\n";   
        return false;
    }
    return true;
}


VictorI2cMaster::VictorI2cMaster()
{
    m_messageStringEnumMap = {
        {"MoveTracksMessage", arduinoIf::arduinoMessageType::MOVE_TRACKS_MESSAGE},
        {"MoveArmMessage", arduinoIf::arduinoMessageType::MOVE_ARM_MESSAGE},
        {"MoveClawMessage", arduinoIf::arduinoMessageType::MOVE_CLAW_MESSAGE}
    };
    m_arduinoMessageSizeMap = {
        {arduinoIf::arduinoMessageType::MOVE_TRACKS_MESSAGE, 3},
        {arduinoIf::arduinoMessageType::MOVE_ARM_MESSAGE, 7},
        {arduinoIf::arduinoMessageType::MOVE_CLAW_MESSAGE, 2}
    };

	init();
}

bool VictorI2cMaster::send(std::string message)
{
    arduinoIf::arduinoMessageType arduinoMessage = extractMessageName(message);
    if(arduinoMessage == arduinoIf::arduinoMessageType::NO_MESSAGE)
    {
        std::cerr << "Received unknown message: " << message <<"\n";
        return false;
    }
    
    std::vector<uint8_t> totalMessage = extractMessageData(message);
    totalMessage.insert(totalMessage.begin(), static_cast<uint8_t>(arduinoMessage));
    if(!isValid(arduinoMessage,totalMessage))
    {
        std::cerr << "Received invalid message: " << message <<"\n";
        return false;
    }

    wiringPiI2CRawWrite(m_fd, &totalMessage[0], totalMessage.size());
    return true;
}
























