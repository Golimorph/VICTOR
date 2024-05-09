
#ifndef VICTOR_ULTRASONIC_H
#define VICTOR_ULTRASONIC_H

#include <Arduino.h>




class VictorUltrasonic
{
public:
    VictorUltrasonic(int TRIG_PIN, int ECHO_PIN);
    long int getDistance();
    
private:
    int m_TRIG_PIN;
    int m_ECHO_PIN;
    int m_obstacleFound;
};

#endif

