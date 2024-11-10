
#ifndef VICTOR_PROGRAMS_H
#define VICTOR_PROGRAMS_H

#include "ServoFunctions.h"

class VictorPrograms
{
public:
    VictorPrograms(ServoFunctions* sf_in)
        :sf(sf_in)
    {
        obstacleAvoidDrive_0_Calls = 0;
    }

    void obstacleAvoidDrive_0(int USdistance)
    {
        obstacleAvoidDrive_0_Calls++;
        if(USdistance<50 && USdistance>2)
        {

            if(abs(sf->getPWM(USERVO_X)-USERVOMID_X)<50)
            {
                MOTOR_GO_BACK;
                delay(50);
            }
            else
            {
                if(sf->getPWM(USERVO_X)<USERVOMID_X)
                {
                    MOTOR_GO_LEFT;
                    delay(50);
                }
                else
                {
                    MOTOR_GO_RIGHT;
                    delay(50);
                }
            }
        }
        else
        {
            MOTOR_GO_FORWARD;
        }
    }


private:
    ServoFunctions * sf;
    int obstacleAvoidDrive_0_Calls;

};


#endif
