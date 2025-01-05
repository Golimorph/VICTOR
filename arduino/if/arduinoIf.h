#pragma once

//Size includes the message type byte at the beginning of the message.
#define SIZE_NO_MESSAGE 1
#define SIZE_MOVE_TRACKS_MESSAGE 3
#define SIZE_MOVE_ARM_MESSAGE 7
#define SIZE_MOVE_CLAW_MESSAGE 2
#define SIZE_MOVE_CLAW_ANGLE_MESSAGE 4
#define SIZE_MOVE_CAMERA_MESSAGE 3

/*! @brief interface for communication between victor raspberrypi and victor arduino.
The interface consist of a number of tasks that victor can performed number in
an enum class. The interface is shared between the raspberrypi and the arduino

This interface is used for communication of I2C. Since only one byte can be
sent over wiringPiI2CWrite(fd, data) at one time, each message consists of a start
byte which depcits  which message will be sent. (the enum). For each message
the size (number of bytes) is specified by the number of entries in that message´s struct.

For example MOVE_TRACKS_MESSAGE is the first message in the arduiniIf enum class.
The corresponding message struct for this message is "MoveTracksMessage" and has 2
attributes, the speed of each track. The message size is therefore 3 bytes. The header
plus the two attributes. So when sending the message to victor, the I2C communication would
look like this:

byte0 = 0x01 -> MOVE_TRACKS_MESSAGE, the arduino gets the value 1, looks it up and knows that MOVE_TRACKS_MESSAGE has size 3 bytes.
byte1 = 0x05 -> leftTrackSpeed
byte2 = 0x01 -> rightTrackSpeed, whend this 3rd byte is received the message reading loop on arduini side will stop and message will be executed.

The arduino then goes back to the state of waiting for a new message, which can also be triggered by sending 0x0 over I2C (NO_MESSAGE).

*/
namespace arduinoIf
{

//Remember to update uart.h,messageHandler.h and victorUart.h if add new messages.
enum class arduinoMessageType
{
    NO_MESSAGE = 0,
    MOVE_TRACKS_MESSAGE = 1,
    MOVE_ARM_MESSAGE = 2,
    MOVE_CLAW_MESSAGE = 3,
    MOVE_CAMERA_MESSAGE = 4
};

struct MoveTracksMessage
{
    char leftTrackSpeed;
    char rightTrackSpeed;
};

struct MoveArmMessage
{
    char a;
    char b;
    char c;
    char d;
    char e;
    char f;
};

struct MoveClawMessage
{
    char state;//0 or 1, open or closed.
};

struct MoveCameraMessage
{
    char xangle;
    char yangle;
};

}//namespace arduinoIf