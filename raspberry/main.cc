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

    std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    //std::string exampleMessage = "MoveTracksMessage(leftTrackSpeed=72, rightTrackSpeed=72)";
    //victorUart.send(exampleMessage);

    VictorServer victorServer(std::bind(&VictorUart::send, &victorUart, std::placeholders::_1));
    victorServer.startI2Cforwarding();

    return 0;
}






/*
    std::cerr<<"done with initialization\n";
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

    int y = 150;
    int increment = 1;
    while(true)
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    
        if(std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count() > 10)
        {
            lastTime = std::chrono::steady_clock::now();

            if(y>300){increment = -1;}
            if(y<150){increment = 1;}
            y=y+increment;
            std::string message = "MoveArmMessage(xcm=0, xmm=0, ycm="+std::to_string(y/10)+", ymm="+std::to_string(y%10)+", zcm=15, zmm=0);";
            std::cerr << message << "\n";
            victorI2cMaster.send(message);
        
           
        }
    }

*/