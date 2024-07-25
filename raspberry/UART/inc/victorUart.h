/*! @brief Class for handling communication between raspberrypi5 and arduino Uno. UART is is more stable than I2C
 * so the project uses this class for the communication.*/

#ifndef VICTORUART_H
#define VICTORUART_H

#include <string>
#include <thread>

#include <string>
#include "raspberryIf.h"
#include <iostream>
#include <map>
#include <vector>
#include "inverseKinematics.h"
#include <cmath> //std::round

class VictorUart {
public:
    VictorUart(const std::string& portName);
    ~VictorUart();
    
    /*! @brief The UART connection to the arduino is used to print debug information. Calling this method starts
     * a separate thread which flushes the rx buffer of the UART to the terminal continously*/
    void startPrintToConsole();

     /*! @brief the send message takes a string which is on kotlin data-class format 
     * and converts it into a series of bytes (first byte denotes which message type) 
     * and the following bytes represent the information in the message. Typically one byte per field of the signal. 
     * It breakes down the message into bytes using the message specification in raspberryIf.h. These are then
     * processed and converted into arduinoIf format and sent to the arduino. */
    bool handleMessage(std::string message);

    /*! @brief preform calculations and send message to arduino for execution */
    bool doMoveTracks(raspberryIf::MoveTracksMessage moveTracksMessage);
    /*! @brief preform calculations and send message to arduino for execution */
    bool doMoveArm(raspberryIf::MoveArmMessage moveArmXYZMessage);
    /*! @brief preform calculations and send message to arduino for execution */
    bool doMoveClaw(raspberryIf::MoveClawMessage moveClawMessage);
    /*! @brief preform calculations and send message to arduino for execution */
    bool doMoveClawAngle(raspberryIf::MoveClawAngleMessage moveArmEulerAngleMessage);

private:
    int fd;
    void configurePort();
    std::string readLine();
    void printToConsoleLoop();
    std::thread m_printToConsoleThread;
    void sendByte(uint8_t byte);
    void send(std::vector<uint8_t> message);
    std::map<std::string, raspberryIf::RaspberryMessageType> m_messageStringEnumMap;//The caller provides a string for the message, this map is used to convert it into the corresponding raspberryMessageType.
    raspberryIf::RaspberryMessageType extractMessageName(const std::string& input);//helper method to get message type from kotlin string
    std::vector<uint8_t> extractMessageData(const std::string& input);   

    InverseKinematics m_inverseKinematics; 
};
    
#endif
