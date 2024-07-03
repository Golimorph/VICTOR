#include "victorUart.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <chrono>
#include <thread>

VictorUart::VictorUart(const std::string& portName) 
{
    

    m_messageStringEnumMap = {
        {"MoveTracksMessage", arduinoIf::arduinoMessageType::MOVE_TRACKS_MESSAGE},
        {"MoveArmMessage", arduinoIf::arduinoMessageType::MOVE_ARM_MESSAGE},
        {"MoveClawMessage", arduinoIf::arduinoMessageType::MOVE_CLAW_MESSAGE}
    };
    m_arduinoMessageSizeMap = {
        {arduinoIf::arduinoMessageType::MOVE_TRACKS_MESSAGE, SIZE_MOVE_TRACKS_MESSAGE},
        {arduinoIf::arduinoMessageType::MOVE_ARM_MESSAGE, SIZE_MOVE_ARM_MESSAGE},
        {arduinoIf::arduinoMessageType::MOVE_CLAW_MESSAGE, SIZE_MOVE_CLAW_MESSAGE}
    };

    fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        std::cerr << "Unable to open port " << portName << std::endl;
        exit(1);
    }

    // Sleep for a short period to give the Arduino time to reset if it does
    std::this_thread::sleep_for(std::chrono::seconds(2));

    configurePort();
}

VictorUart::~VictorUart()
{
    close(fd);
}

void VictorUart::configurePort() 
{
    termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;

    tcsetattr(fd, TCSANOW, &options);
}

void VictorUart::sendByte(uint8_t byte) 
{
    write(fd, &byte, sizeof(byte));
}

std::string VictorUart::readLine() 
{
    std::string result;
    char buffer[1];
    while (true) {
        int n = read(fd, buffer, 1);
        if (n > 0) {
            if (buffer[0] == '\n') break;
            result += buffer[0];
        } else {
            break;
        }
    }
    return result;
}

void VictorUart::printToConsoleLoop()
{
    while (true) 
    {
        // Read the response
        std::string response = readLine();
        if (!response.empty()) {
            std::cerr << "Arduino: " << response << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void VictorUart::startPrintToConsole()
{
    m_printToConsoleThread = std::thread(&VictorUart::printToConsoleLoop, this);
}


arduinoIf::arduinoMessageType VictorUart::extractMessageName(const std::string& input) 
{
    size_t pos = input.find('(');//can assume to always work since string is kotlin-data-class toString output.
    std::string messageName = input.substr(0, pos);


    if(m_messageStringEnumMap.find(messageName) == m_messageStringEnumMap.end())
    {
        return arduinoIf::arduinoMessageType::NO_MESSAGE;    
    }

    return m_messageStringEnumMap.at(messageName);
   
}

std::vector<uint8_t> VictorUart::extractMessageData(const std::string& input) {
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

bool VictorUart::isValid(arduinoIf::arduinoMessageType arduinoMessage, std::vector<uint8_t> totalMessage)
{
    if(m_arduinoMessageSizeMap.at(arduinoMessage)!=totalMessage.size())
    {
        std::cerr<<"Raspberry: The message size is invalid, expected " << m_arduinoMessageSizeMap.at(arduinoMessage) << " bytes, " << " but only managed to parse " << totalMessage.size() << "bytes from the string (message type included)\n";   
        return false;
    }
    return true;
}

bool VictorUart::send(std::string message)
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

    for(uint8_t &byte : totalMessage)
    {
        sendByte(byte);
    }

    return true;
}










