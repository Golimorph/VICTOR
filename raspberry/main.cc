//created by Richard Edberg 2024
#include "victorI2cMaster.h"
#include <string>
#include "victorServer.h"


int main() 
{
	    
    VictorI2cMaster victorI2cMaster;
    VictorServer victorServer(std::bind(&VictorI2cMaster::send, &victorI2cMaster, std::placeholders::_1));
    victorServer.startI2Cforwarding();

    return 0;
}
