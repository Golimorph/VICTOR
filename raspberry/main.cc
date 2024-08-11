//created by Richard Edberg 2024
#include <string>
#include "victorServer.h"
#include "victorUart.h"
#include <iostream>




int main() 
{
    std::cerr<<"raspberry: started\n";
    VictorUart victorUart("/dev/ttyUSB0");
    victorUart.startPrintToConsole();

    raspberryIf::MoveArmMessage moveArmMessage;
    moveArmMessage.xcm = 0; 
    moveArmMessage.xmm = 0;
    moveArmMessage.ycm = 20;
    moveArmMessage.ymm = 0;
    moveArmMessage.zcm = 9;
    moveArmMessage.zmm = 3;
    victorUart.doMoveArm(moveArmMessage);
    
    
    VictorServer victorServer(std::bind(&VictorUart::handleMessage, &victorUart, std::placeholders::_1));
    victorServer.startI2Cforwarding();

    return 0;
}

