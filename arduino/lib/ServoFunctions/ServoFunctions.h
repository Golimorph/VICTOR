
#ifndef SERVO_FUNCTIONS_H
#define SERVO_FUNCTIONS_H

//#include "Arduino.h"
//#include "protothreads.h"
//#include <Servo.h>
#include <string.h>
#include <Adafruit_PWMServoDriver.h>
#include "../Arduino_AVRSTL/src/vector"


//Each instance of this calls is a thread that can control one servo.
//To move several servos at the same time, multiple instances are needed.





//MOTOR CONTROLS
//#define PWMD 3 //D电机转速
//#define DIRD 2 //D电机转向
//#define PWMC 5 //C电机转速
//#define DIRC 4 //C电机转向
#define PWMB 6 //B电机转速
#define DIRB 7 //B电机转向
#define PWMA 9 //A电机转速
#define DIRA 8 //A电机转向
//Motor hall sensors
#define outputMotorHallRightA 10
#define outputMotorHallRightB 11
#define outputMotorHallLeftA 12
#define outputMotorHallLeftB 13



#define MOTOR_GO_FORWARD  {digitalWrite(DIRA,HIGH);analogWrite(PWMA,100);digitalWrite(DIRB,LOW);analogWrite(PWMB,100);} //车体前进
#define MOTOR_GO_BACK   {digitalWrite(DIRA,LOW);analogWrite(PWMA,100);digitalWrite(DIRB,HIGH);analogWrite(PWMB,100);}   //车体后退
#define MOTOR_GO_LEFT   {digitalWrite(DIRA,LOW);analogWrite(PWMA,100);digitalWrite(DIRB,LOW);analogWrite(PWMB,100);}  //车体左转
#define MOTOR_GO_RIGHT    {digitalWrite(DIRA,HIGH);analogWrite(PWMA,100);digitalWrite(DIRB,HIGH);analogWrite(PWMB,100);}  //车体右转
#define MOTOR_GO_STOP   {digitalWrite(DIRA,LOW);analogWrite(PWMA,0);digitalWrite(DIRB,LOW);analogWrite(PWMB,0);}       //车体静止





#define MAX_PACKETSIZE 32

//SEROV CONTROLS

#define SERVOMIN 125
#define SERVOMAX 625
#define SERVOMID 375

//ARM SERVOS
#define ROTSERVO 2
#define ROTSERVO_MIN 250
#define ROTSERVO_MID 400
#define ROTSERVO_MAX 500

#define SHOULDERSERVOLEFT 4
#define SHOULDERSERVOLEFT_MIN 0
#define SHOULDERSERVOLEFT_MID 350
#define SHOULDERSERVOLEFT_MAX 1000
#define SHOULDERSERVORIGHT 5
#define SHOULDERSERVORIGHT_MIN 190 //-65.7 degrees
#define SHOULDERSERVORIGHT_MID 350
#define SHOULDERSERVORIGHT_MAX 575
#define SHOULDERSERVO_ZEROANGLE_UPRIGHT 356 //0 degrees


#define ELBOWSERVO 11
#define ELBOWSERVO_MIN 240 //-31.9 degrees
#define ELBOWSERVO_MID 350 //0, perpendicular to shoudler arm.
#define ELBOWSERVO_MAX 525 //70.1 degrees

#define UNDERARMROTSERVO 6
#define UNDERARMROTSERVO_MIN 125 //97.7 degrees anti-clockwise.(82.3)
#define UNDERARMROTSERVO_MID 363 //0 degrees Horizonal.
#define UNDERARMROTSERVO_MAX 600 //94.8 degrees clockwise. (85.2)

#define WRISTUPSERVO 7
#define WRISTUPSERVO_MIN 125
#define WRISTUPSERVO_MID 335
#define WRISTUPSERVO_MAX 600

#define WRISTTWISTSERVO 10
#define WRISTTWISTSERVO_MIN 125
#define WRISTTWISTSERVO_MID 370
#define WRISTTWISTSERVO_MAX 600

#define CLAWSERVO 9
#define CLAWSERVO_MIN 260
#define CLAWSERVO_MID 400
#define CLAWSERVO_MAX 500


//ULTRASONIC SENSOR SERVOS.
#define USERVO_X 0
#define USERVO_Y 1
#define USERVOMIN_X 140
#define USERVOMID_X 385
#define USERVOMAX_X 600
#define USERVOMIN_Y 250
#define USERVOMID_Y 330
#define USERVOMAX_Y 410


class ServoFunctions
{
public:
    ServoFunctions(const int, const int);
    
    int USdistance=1000;
    
    void setup();
    void smoothMove(const int servoNumber, const int newPulse, const int speed);
    bool smoothMove_pulse(const int servoNumber, const int newPulse, const int speed, const int thread, const int task);
    void rsmoothMove_pulse(const int servoNumber, const int newPulse, const int speed, const int thread, const int task);
    
    int smoothMove_pulsePT(struct pt* pt);
    int getA();
    void moveUSInCircle(int speed);
    int updateMotorsHallSensorPosition(struct pt* pt);
    int getMotorPosLeft();
    int getMotorPosRight();
    int getPWM(int servoNumber);
    int obstacleAvoidDrive(struct pt* pt);
    void resetTasks(int thread);
    
    unsigned long long int cycleClock;
private:
    
    std::vector<std::vector<bool>> m_taskComplete;
    std::vector<long int> m_sleepValues;
    bool sleep(const long int cycles, const int thread, const int task);
    
    void ServoFunctions::initMotorHallSensors();
	char buffUART[MAX_PACKETSIZE];
	unsigned int buffUARTIndex = 0;
	unsigned long preUARTTick = 0;
    Adafruit_PWMServoDriver adafruit_PWMServoDriver;
    bool setPWM(const int servoNumber, const int value);
    int currentPWMs [10];

    //Motors Hall sensors
    int m_positionMotorHallRight = 0;
    int m_aStateMotorHallRight;
    int m_aLastStateMotorHallRight;
    int m_positionMotorHallLeft = 0;
    int m_aStateMotorHallLeft;
    int m_aLastStateMotorHallLeft;
    
    //Moving servo protothread variables:
    
    
    

};

#endif

