/* Interface for communication between raspberrypi and external part.
 * From the Socket the raspberryPi gets strings and in data class format
 * from kotlin. In the victorServer these strings are parsed and converted
 * into structs representing the message from the external part.
 *
 * The messages from the external part can the be handled by the message
 * handler which converts the desired message into arduinoIf equivalent and
 * sent to the arduino.
 *
 * This is needed because raspberryIf messages cannot be sent directly to
 * the arduiono as they contain higher level information and processing
 * is needed to convert it to a format that arduiono can execute. Some
 * messages are already on arduino level format and dont need any processing
 * so for those messages it is kind of unecessary but still kept here
 * to be consequent.
 */
#pragma once
#include <utility> //std::pair
#include <optional> //std::optional
#include <vector>
#include <iostream>
#include <cstring> //std::memcpy

//Size includes the message type byte at the beginning of the message.
#define RASPBERRY_SIZE_NO_MESSAGE 1
#define RASPBERRY_SIZE_MOVE_TRACKS_MESSAGE 3
#define RASPBERRY_SIZE_MOVE_ARM_MESSAGE 7
#define RASPBERRY_SIZE_MOVE_CLAW_MESSAGE 2
#define RASPBERRY_SIZE_MOVE_CLAW_ANGLE_MESSAGE 4
#define RASPBERRY_SIZE_MOVE_CAMERA_MESSAGE 3

namespace raspberryIf
{

//Remember to update victorUart.h (the message handler in raspberry) if add new messages.
enum class RaspberryMessageType
{
    NO_MESSAGE = 0,
    MOVE_TRACKS_MESSAGE = 1,
    MOVE_ARM_MESSAGE = 2,
    MOVE_CLAW_MESSAGE = 3,
    MOVE_CAMERA_MESSAGE = 4,
    MOVE_CLAW_ANGLE_MESSAGE = 5
};

#pragma pack(push, 1) //sets the alignment to 1 byte and saves the current alignment settings
//This ensures that the structs have no padding bytes, which makes them match the layout of the incoming data exactly.

struct MoveTracksMessage
{
    char leftTrackSpeed;
    char rightTrackSpeed;
};

struct MoveArmMessage
{
    char xcm;
    char xmm;
    char ycm;
    char ymm;
    char zcm;
    char zmm;
};

struct MoveClawMessage
{
    char state;//0 or 1, open or closed.
};

struct MoveClawAngleMessage
{
    char alpha;
    char beta;
    char gamma;
};

struct MoveCameraMessage
{
    char xangle;
    char yangle;
};

#pragma pack(pop) //restores the previous alignment settings after the struct definitions.


/* @brief create one of the above defined messages from a vector of bytes*/
template <typename MessageType>
std::optional<MessageType> createMessage(const std::vector<int8_t> messageData)
{
    if (messageData.size() != sizeof(MessageType))
    {
        std::cerr << "Message data vector is not same size as struct! " << messageData.size() << ", " << sizeof(MessageType) << "\n";
        return std::nullopt;
    }

    MessageType messageType;
    std::memcpy(&messageType, messageData.data(), sizeof(MessageType));
    return messageType;
}

}//namespace raspberryIf