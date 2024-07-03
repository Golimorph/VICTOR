#ifndef VICTORUART_H
#define VICTORUART_H

#include <string>
#include <thread>

#include <string>
#include "arduinoIf.h"
#include <iostream>
#include <map>
#include <vector>

class VictorUart {
public:
    VictorUart(const std::string& portName);
    ~VictorUart();
    
    void startPrintToConsole();

     /*! @brief the send message takes a string which is on kotlin data-class format 
     * and converts it into a series of bytes (first byte denotes which message type) 
     * and the following bytes represent the information in the message. Typically one byte per field of the signal. 
     * It breakes down the message into bytes using the message specification in arduinoIf.h*/
    bool send(std::string message);

private:
    void sendByte(uint8_t byte);
    std::string readLine();
    int fd;
    void configurePort();
    void printToConsoleLoop();
    std::thread m_printToConsoleThread;

    bool isValid(arduinoIf::arduinoMessageType arduinoMessage, std::vector<uint8_t> totalMessage);
    arduinoIf::arduinoMessageType extractMessageName(const std::string& input);//helper method to get message type from kotlin string
    std::vector<uint8_t> extractMessageData(const std::string& input);
    std::map<std::string, arduinoIf::arduinoMessageType> m_messageStringEnumMap;//The caller provides a string for the message, but in I2C an enum is used as it is less bug prone.
    std::map<arduinoIf::arduinoMessageType, uint8_t> m_arduinoMessageSizeMap;//The number of bytes to be sent over I2C for each message.
};
    



#endif
