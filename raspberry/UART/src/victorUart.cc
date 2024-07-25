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
        {"MoveTracksMessage", raspberryIf::RaspberryMessageType::MOVE_TRACKS_MESSAGE},
        {"MoveArmMessage", raspberryIf::RaspberryMessageType::MOVE_ARM_MESSAGE},
        {"MoveClawMessage", raspberryIf::RaspberryMessageType::MOVE_CLAW_MESSAGE},
        {"MoveClawAngleMessage", raspberryIf::RaspberryMessageType::MOVE_CLAW_ANGLE_MESSAGE}
    };

    fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) 
    {
        std::cerr << "Unable to open port " << portName << std::endl;
        exit(1);
    }

    // Sleep for a short period to give the raspberry time to reset if it does
    std::this_thread::sleep_for(std::chrono::seconds(2));

    configurePort();

    m_inverseKinematics = InverseKinematics();
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


raspberryIf::RaspberryMessageType VictorUart::extractMessageName(const std::string& input) 
{
    size_t pos = input.find('(');//can assume to always work since string is kotlin-data-class toString output.
    std::string messageName = input.substr(0, pos);


    if(m_messageStringEnumMap.find(messageName) == m_messageStringEnumMap.end())
    {
        return raspberryIf::RaspberryMessageType::NO_MESSAGE;    
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


bool VictorUart::handleMessage(std::string message)
{
    auto messageType = extractMessageName(message);
    auto messageData = extractMessageData(message);
    
    if(messageType == raspberryIf::RaspberryMessageType::MOVE_TRACKS_MESSAGE)
    {
        auto moveTracksMessage = raspberryIf::createMessage<raspberryIf::MoveTracksMessage>(messageData);
        return moveTracksMessage.has_value() ? doMoveTracks(moveTracksMessage.value()) : false;
    }
    else if(messageType == raspberryIf::RaspberryMessageType::MOVE_ARM_MESSAGE)
    {
        auto moveArmMessage = raspberryIf::createMessage<raspberryIf::MoveArmMessage>(messageData);
        return moveArmMessage.has_value() ? doMoveArm(moveArmMessage.value()) : false;
    }
    else if(messageType == raspberryIf::RaspberryMessageType::MOVE_CLAW_MESSAGE)
    {
        auto moveClawMessage = raspberryIf::createMessage<raspberryIf::MoveClawMessage>(messageData);
        return moveClawMessage.has_value() ? doMoveClaw(moveClawMessage.value()) : false;
    }
    else
    {
        std::cerr << "Raspberry: Received unknown message: " << message <<"\n";
        return false;
    }   
}

bool VictorUart::doMoveTracks(raspberryIf::MoveTracksMessage moveTracksMessage)
{
    std::vector<uint8_t> message{static_cast<uint8_t>(raspberryIf::RaspberryMessageType::MOVE_TRACKS_MESSAGE), moveTracksMessage.leftTrackSpeed, moveTracksMessage.rightTrackSpeed};
    send(message);
    return true;
}

bool VictorUart::doMoveArm(raspberryIf::MoveArmMessage moveArmMessage)
{
    
    double x = moveArmMessage.xcm*10 + moveArmMessage.xmm;
    double y = moveArmMessage.ycm*10 + moveArmMessage.ymm;
    double z = moveArmMessage.zcm*10 + moveArmMessage.zmm;
    double alpha = 90.0;
    double beta = 0;
    double gamma = 0;
   
    const std::vector<double> desiredValue{x,y,z,alpha,beta,gamma};
    std::vector<double> solution;
    m_inverseKinematics.solve(desiredValue, solution);

    std::vector<uint8_t> solutionU8;
    solutionU8.push_back(static_cast<uint8_t>(raspberryIf::RaspberryMessageType::MOVE_ARM_MESSAGE));

    for(const double& val : solution)
    {
        solutionU8.push_back(static_cast<uint8_t>(static_cast<int8_t>(std::round(val))));
    }
    send(solutionU8);

    return true;
}

bool VictorUart::doMoveClaw(raspberryIf::MoveClawMessage moveClawMessage)
{
    std::vector<uint8_t> message{static_cast<uint8_t>(raspberryIf::RaspberryMessageType::MOVE_CLAW_MESSAGE), moveClawMessage.state};
    send(message);
    return true;
}

void VictorUart::send(std::vector<uint8_t> message)
{
    std::cerr << "Raspberry: sending Message: ";
    for(uint8_t &byte : message)
    {
        std::cerr << static_cast<int>(byte) << ", ";
        sendByte(byte);
    }
    std::cerr << "\n";
}
