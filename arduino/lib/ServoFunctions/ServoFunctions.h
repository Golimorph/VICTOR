//created by Richard Edberg 2021
#ifndef SERVO_FUNCTIONS_H
#define SERVO_FUNCTIONS_H

//#include "Arduino.h"
//#include "protothreads.h"
//#include <Servo.h>
#include <string.h>
#include <Adafruit_PWMServoDriver.h>
#include "../Arduino_AVRSTL/src/vector"
#include <inverseKinematics.h>


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

#define MAX_PACKETSIZE 32

//SERVO CONTROLS
#define NUMBER_OF_SERVOS 12

#define SERVOMIN 125
#define SERVOMAX 625
#define SERVOMID 375

//ARM SERVOS
#define ROTSERVO 2
#define ROTSERVO_MIN 250
#define ROTSERVO_MID 400
#define ROTSERVO_MAX 500

/*calibrated ** */
#define SHOULDERSERVOLEFT 4 //Do not use
#define SHOULDERSERVOLEFT_MIN 0//use RIGHT for setting both
#define SHOULDERSERVOLEFT_MID 350 //only for calibration
#define SHOULDERSERVOLEFT_MAX 1000 //use RIGHT for setting both
#define SHOULDERSERVORIGHT 5 //Do use please
#define SHOULDERSERVORIGHT_MIN 190 //-65.5 degrees
#define SHOULDERSERVORIGHT_MID 350 //only for calibration
#define SHOULDERSERVORIGHT_MAX 575 // 88.0 degrees 
#define SHOULDERSERVO_ZEROANGLE_UPRIGHT 356 //0 degrees, use this

/*calibrated ** */
#define ELBOWSERVO 11
#define ELBOWSERVO_MIN 240 //-32.5 degrees
#define ELBOWSERVO_MID 350 //0, perpendicular to shoudler arm.
#define ELBOWSERVO_MAX 550 //525 is 68.3 degrees

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

//resting position 
//sf.moveServo(SHOULDERSERVORIGHT, -80, 3000);
//sf.moveServo(ELBOWSERVO, 75, 3000);


class ServoFunctions
{
public:
    ServoFunctions();

    /*! @brief sets up the communication with the servo shield and initializes 
     * all servos to their default position.*/
    void setup();


    /*! @brief move the arm to coordinates x,y,z relative to the
     * center of victor.
     * @param x distance to the right of victor center[mm]
     * @param y distance in front of victor center [mm]
     * @param z distance above victor center [mm]
     * @param time, the time it shall take to complete the move to the new coordinates [ms]*/
    bool moveArm(const double x, const double y, const double z, const int time);
    
    /*! @brief moves a servo from current angle to new angle.
     * This method sets the private variable desiredPWM and the millisecondsPerPWMStep (speed)
     * The method call is non-blocking and the actual servo movement is done by increments
     * in the arduino loop() context.
     * @param servoNumber which servo
     * @param angle the new angle for the servo
     * @param time the time in ms it shall take for the servo to reach the new position (i.e. speed of the movement)*/
    void moveServo(const int servoNumber, const double angle, const int time);
  
  
    /*! @brief set the speed of the two tracks of victor
     * speedA left track speed
     * speedB right track speed*/
    void setMotorSpeed(int speedA, int speedB);


    int getMotorPosLeft();
    int getMotorPosRight();
    int getPWM(int servoNumber);

    /*! @brief perform a simple nodding gesture of the US sensor to show 
     * that initialization succedded, only run this in setup(), as it 
     * is a blocking call */
    void indInitCompleted();

    
    /*! @brief this void is called once in each arduino loop and ensures that each 
     * servo is moving toward the desired PWM with the speed millisecondsPerStep
     * and that all functions are updated. It is needed because arduino does not 
     * support threading in a simple way, and this instead acts as sort of a polling technique*/
    void refresh();
    
private:

    //Refresh methods and variables:
    void updateServoPositions();
    
    //help methods and variables:
    void initMotorHallSensors();
    bool setPWM(const int servoNumber, const int value);
    int angleToPWM(const int servoNumber, const double angle) const;
    InverseKinematics m_inverseKinematics;
    
    //library variables:
    Adafruit_PWMServoDriver adafruit_PWMServoDriver;
    
    //State variables:
    int currentPWMs[NUMBER_OF_SERVOS]; //The current position of each servo
    int desiredPWMs[NUMBER_OF_SERVOS]; //The desired position of each servo
    unsigned int millisecondsPerPWMStep[NUMBER_OF_SERVOS] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};//The speed of each servo, can be positive or negative.
    unsigned long lastPWMupdateTime[NUMBER_OF_SERVOS] = {0,0,0,0,0,0,0,0,0,0,0,0};//The time in ms from arduino start when each servo was updated.



    int m_positionMotorHallRight = 0; //Motors Hall sensors
    int m_aStateMotorHallRight; //Motors Hall sensors
    int m_aLastStateMotorHallRight; //Motors Hall sensors
    int m_positionMotorHallLeft = 0; //Motors Hall sensors
    int m_aStateMotorHallLeft; //Motors Hall sensors
    int m_aLastStateMotorHallLeft; //Motors Hall sensors

};

#endif

