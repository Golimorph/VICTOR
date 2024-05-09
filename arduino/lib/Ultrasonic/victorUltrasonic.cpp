

#include "victorUltrasonic.h"


VictorUltrasonic::VictorUltrasonic(int TRIG_PIN, int ECHO_PIN)
{
    m_TRIG_PIN=TRIG_PIN;
    m_ECHO_PIN=ECHO_PIN;
    pinMode(m_TRIG_PIN,OUTPUT);
    pinMode(m_ECHO_PIN,INPUT);
}

long int VictorUltrasonic::getDistance()
{
    long int duration, distance;
    digitalWrite(m_TRIG_PIN, LOW);
    delayMicroseconds(20);
    digitalWrite(m_TRIG_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(m_TRIG_PIN, LOW);
    duration = pulseIn(m_ECHO_PIN,HIGH);
    if(duration==0)
    {
        return -1;//unsuccessful read
    }
    distance = duration / 29.1 / 2.0 ;
    
    return distance;
}

