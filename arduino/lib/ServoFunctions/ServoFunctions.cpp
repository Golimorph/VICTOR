

#include "ServoFunctions.h"


ServoFunctions::ServoFunctions()
{
}

int ServoFunctions::getPWM(int servoNumber)
{
    return currentPWMs[servoNumber];
}

int ServoFunctions::getMotorPosLeft()
{
    return m_positionMotorHallLeft;
}

int ServoFunctions::getMotorPosRight()
{
    return m_positionMotorHallRight;
}

void ServoFunctions::setup()
{
    adafruit_PWMServoDriver=Adafruit_PWMServoDriver::Adafruit_PWMServoDriver();
    adafruit_PWMServoDriver.begin();
    adafruit_PWMServoDriver.setPWMFreq(60);//DO NOT CHANGE!!

    setPWM(USERVO_X,USERVOMID_X);
    setPWM(USERVO_Y,USERVOMID_Y);
    setPWM(ROTSERVO,ROTSERVO_MID);
    setPWM(SHOULDERSERVORIGHT,SHOULDERSERVORIGHT_MIN);
    setPWM(SHOULDERSERVOLEFT,SHOULDERSERVOLEFT_MID+(SHOULDERSERVORIGHT_MID-SHOULDERSERVORIGHT_MIN));
    setPWM(ELBOWSERVO,ELBOWSERVO_MAX);
    setPWM(UNDERARMROTSERVO,UNDERARMROTSERVO_MID);
    setPWM(WRISTUPSERVO,WRISTUPSERVO_MID);
    setPWM(WRISTTWISTSERVO,WRISTTWISTSERVO_MID);
    setPWM(CLAWSERVO,CLAWSERVO_MAX);

    pinMode(DIRA,OUTPUT);
    pinMode(PWMA,OUTPUT);
    pinMode(DIRB,OUTPUT);
    pinMode(PWMB,OUTPUT);
    
    initMotorHallSensors();

    for (int servo = 0; servo < NUMBER_OF_SERVOS; ++servo)
    {
        desiredPWMs[servo] = currentPWMs[servo];
    }
}

//MotorHallSensor
void ServoFunctions::initMotorHallSensors()
{
  pinMode (outputMotorHallRightA,INPUT);
  pinMode (outputMotorHallRightB,INPUT);
  pinMode (outputMotorHallLeftA,INPUT);
  pinMode (outputMotorHallLeftB,INPUT);
  m_aLastStateMotorHallRight = digitalRead(outputMotorHallRightA);
  m_aLastStateMotorHallLeft = digitalRead(outputMotorHallLeftA);
}
/*
int ServoFunctions::updateMotorsHallSensorPosition(struct pt* pt)
{
    PT_BEGIN(pt);
    while(true)
    {
    //Right
      m_aStateMotorHallRight = digitalRead(outputMotorHallRightA);
       if (m_aStateMotorHallRight != m_aLastStateMotorHallRight){
         if (digitalRead(outputMotorHallRightB) != m_aStateMotorHallRight) {
           m_positionMotorHallRight --;
         } else {
           m_positionMotorHallRight ++;
         }
       }
       m_aLastStateMotorHallRight = m_aStateMotorHallRight;
    //Left
       m_aStateMotorHallLeft = digitalRead(outputMotorHallLeftA);
       
       if (m_aStateMotorHallLeft != m_aLastStateMotorHallLeft){
         if (digitalRead(outputMotorHallLeftB) != m_aStateMotorHallLeft) {
           m_positionMotorHallLeft ++;
         } else {
           m_positionMotorHallLeft --;
         }
       }
       m_aLastStateMotorHallLeft = m_aStateMotorHallLeft; // Updates the previous state of the outputA with the current state
       PT_YIELD(pt);
    }
    PT_END(pt);
}
 */

int ServoFunctions::angleToPWM(const int servoNumber, const double angle) const
{
    switch(servoNumber)
    {   
    case ROTSERVO:
        return int(-angle*2.48 +0.5) + ROTSERVO_MID;
    case SHOULDERSERVORIGHT: 
        if(angle>0)//try to compensate backlash, gravity is different in the two directions.
        {
            return int(angle*2.56 + 0.5) + SHOULDERSERVO_ZEROANGLE_UPRIGHT;
        }else
        {
            return int(angle*2.43 + 0.5) + SHOULDERSERVO_ZEROANGLE_UPRIGHT;
        }
    case ELBOWSERVO:
        return int(-0.00813*angle*angle + 3.08*angle + 0.5) + ELBOWSERVO_MID;
    case UNDERARMROTSERVO: 
        return int(angle*2.48 + 0.5) + UNDERARMROTSERVO_MID;
    case WRISTUPSERVO: 
        return int(angle*2.48 + 0.5) + WRISTUPSERVO_MID;
    case WRISTTWISTSERVO: 
        return int(angle*2.48 + 0.5) + WRISTTWISTSERVO_MID;
    case CLAWSERVO: 
        return int(angle*2.48 + 0.5) + CLAWSERVO_MID;
    }
}

bool ServoFunctions::setPWM(const int servoNumber, const int value)
{

    switch(servoNumber)
    {
        case USERVO_X:
            if(value>=USERVOMIN_X && value<=USERVOMAX_X)
            {
             adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
             currentPWMs[servoNumber]=value;
             return true;
            }
            return false;
        case USERVO_Y:
            if(value>=USERVOMIN_Y && value<=USERVOMAX_Y)
            {
                adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
                currentPWMs[servoNumber]=value;
                return true;
            }
            return false;
        case ROTSERVO:
            if(value>=ROTSERVO_MIN && value<=ROTSERVO_MAX)
            {
                adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
                currentPWMs[servoNumber]=value;
                return true;
            }
            return false;
        case SHOULDERSERVORIGHT:
            if(value>=SHOULDERSERVORIGHT_MIN && value<=SHOULDERSERVORIGHT_MAX)
            {
                int valueLeft=SHOULDERSERVOLEFT_MID+(SHOULDERSERVORIGHT_MID-value);
                if(valueLeft>=SHOULDERSERVOLEFT_MIN && valueLeft<=SHOULDERSERVOLEFT_MAX)
                {
                    adafruit_PWMServoDriver.setPWM(SHOULDERSERVORIGHT, 0, value);
                    currentPWMs[SHOULDERSERVORIGHT]=value;
                    adafruit_PWMServoDriver.setPWM(SHOULDERSERVOLEFT, 0, valueLeft);
                    currentPWMs[SHOULDERSERVOLEFT]=valueLeft;
                    return true;
                }
            }
            return false;
        case ELBOWSERVO:
            if(value>=ELBOWSERVO_MIN && value<=ELBOWSERVO_MAX)
            {
                adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
                currentPWMs[servoNumber]=value;
                return true;
            }
            return false;
        case UNDERARMROTSERVO:
            if(value>=UNDERARMROTSERVO_MIN && value<=UNDERARMROTSERVO_MAX)
            {
                adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
                currentPWMs[servoNumber]=value;
                return true;
            }
            return false;
        case WRISTUPSERVO:
            if(value>=WRISTUPSERVO_MIN && value<=WRISTUPSERVO_MAX)
            {
                adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
                currentPWMs[servoNumber]=value;
                return true;
            }
            return false;
        case WRISTTWISTSERVO:
            if(value>=WRISTTWISTSERVO_MIN && value<=WRISTTWISTSERVO_MAX)
            {
                adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
                currentPWMs[servoNumber]=value;
                return true;
            }
            return false;
        case CLAWSERVO:
            if(value>=CLAWSERVO_MIN && value<=CLAWSERVO_MAX)
            {
                adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
                currentPWMs[servoNumber]=value;
                return true;
            }
            return false;
        default:
            return true;
    }
}


bool ServoFunctions::moveClawAngle(const double x, const double y, const double z, const int time)
{
    double omega,psi;
    if(!m_inverseKinematics.getClawAngle(omega, psi, x,y,z))
    {
        return false;
    }

    moveServo(UNDERARMROTSERVO, psi, time);
    moveServo(WRISTUPSERVO, omega, time);
    
    return true;
}


bool ServoFunctions::moveArm(const double x, const double y, const double z, const int time)
{
    double shoulderServoAngle,elbowServoAngle,rotAngle;

    if(!m_inverseKinematics.solve(shoulderServoAngle, elbowServoAngle, rotAngle, x, y, z))
    {
        return false;
    }

    moveServo(ROTSERVO, rotAngle, time);
    moveServo(SHOULDERSERVORIGHT, shoulderServoAngle, time);
    moveServo(ELBOWSERVO, elbowServoAngle , time);

    return true;
}




void ServoFunctions::moveServo(const int servoNumber, const double angle, const int time)
{
	int desiredPWM = angleToPWM(servoNumber,angle);

    if(time == 0)
    {
        desiredPWMs[servoNumber] = desiredPWM;
        currentPWMs[servoNumber] = desiredPWM;
        setPWM(servoNumber, desiredPWM);
    }else
    {
        int numberOfPWMsteps = abs(desiredPWM-currentPWMs[servoNumber]);
        if(numberOfPWMsteps < 2)
        {   
            //In case there is no change in servo position, return directly.
            return;
        }
        millisecondsPerPWMStep[servoNumber] = time/numberOfPWMsteps;
        desiredPWMs[servoNumber] = desiredPWM;
    }
}


void ServoFunctions::refresh()
{
    updateServoPositions();
}

void ServoFunctions::updateServoPositions()
{
    unsigned long time = millis();

    for(int servoNumber = 0; servoNumber < NUMBER_OF_SERVOS; ++servoNumber)
    { 
        int deltaPWM = desiredPWMs[servoNumber]-currentPWMs[servoNumber];
        
        if(servoNumber != SHOULDERSERVOLEFT && abs(deltaPWM)>2)
        {
            if((time - lastPWMupdateTime[servoNumber]) > millisecondsPerPWMStep[servoNumber])
            {
                lastPWMupdateTime[servoNumber] = time;

                if(deltaPWM > 0)
                {
                    setPWM(servoNumber, currentPWMs[servoNumber] + 1);
                }else
                {
                    setPWM(servoNumber, currentPWMs[servoNumber] - 1);
                }
            }
        }
    }
}

void ServoFunctions::setMotorSpeed(int speedA, int speedB)
{
    const double speedMultiplier = 1.3;
    const int maxSpeed = 120;
    speedA = static_cast<int>(static_cast<double>(speedA)*speedMultiplier);
    speedB = static_cast<int>(static_cast<double>(speedB)*speedMultiplier);

    //This set the direction of the motor forward or backward. A negative speed means to go backward
    if(speedA >= 0)
    {
        digitalWrite(DIRA,HIGH);
    }else
    {
        digitalWrite(DIRA,LOW);
    }
    analogWrite(PWMA, std::min(abs(speedA),maxSpeed));

    //This set the direction of the motor forward or backward. A negative speed means to go backward
    if(speedB >= 0)
    {
        digitalWrite(DIRB,LOW);
    }else{
        digitalWrite(DIRB,HIGH);
    }
    analogWrite(PWMB, std::min(abs(speedB),maxSpeed));
}





void ServoFunctions::indInitCompleted()
{
    setPWM(USERVO_Y,USERVOMAX_Y);
    delay(300);
    setPWM(USERVO_Y,USERVOMIN_Y);
    delay(300);
    setPWM(USERVO_Y,USERVOMID_Y);
}

