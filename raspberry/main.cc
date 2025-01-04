//created by Richard Edberg 2024
#include <string>
#include "victorServer.h"
#include "victorUart.h"
#include "stateMachine.h"
#include "camera.h"
#include <iostream>
#include "trace.h"





int main()
{
    INFO("raspberry: started");
    VictorUart victorUart("/dev/ttyUSB0");
    victorUart.startPrintToConsole();
    Camera camera;//Camera interface to the Hailo python API.

    raspberryIf::MoveArmMessage moveArmMessage;
    moveArmMessage.xcm = 0;
    moveArmMessage.xmm = 0;
    moveArmMessage.ycm = 20;
    moveArmMessage.ymm = 0;
    moveArmMessage.zcm = 9;
    moveArmMessage.zmm = 3;
    victorUart.doMoveArm(moveArmMessage);

    StateMachine stateMachine(victorUart, camera);
    stateMachine.start();


    VictorServer victorServer(std::bind(&VictorUart::handleMessage, &victorUart, std::placeholders::_1));
    victorServer.startI2Cforwarding();//blocking

    return 0;
}

