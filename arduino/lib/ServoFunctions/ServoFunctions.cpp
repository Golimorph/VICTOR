

#include "ServoFunctions.h"


ServoFunctions::ServoFunctions(const int threads, const int tasks)
{
    cycleClock=0;
    
    for(int i=0; i<threads; i++)
    {
        m_taskComplete.push_back({true});
        for(int j=1; j<tasks; j++)
        {
            m_taskComplete[i].push_back(false);
        }
        m_sleepValues.push_back(0);
    }
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

void ServoFunctions::resetTasks(int thread)
{
    m_taskComplete[thread]={true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
}




void ServoFunctions::setup()
{
    adafruit_PWMServoDriver=Adafruit_PWMServoDriver::Adafruit_PWMServoDriver();
    adafruit_PWMServoDriver.begin();
    adafruit_PWMServoDriver.setPWMFreq(60);

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

    //TRACK MOTOR CONTROLS
    pinMode(DIRA,OUTPUT);
    pinMode(PWMA,OUTPUT);
    pinMode(DIRB,OUTPUT);
    pinMode(PWMB,OUTPUT);
    //pinMode(DIRC,OUTPUT);
    //pinMode(PWMC,OUTPUT);
    //pinMode(DIRD,OUTPUT);
    //pinMode(PWMD,OUTPUT);
    initMotorHallSensors();
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

bool ServoFunctions::setPWM(const int servoNumber, const int value)
{
	if(servoNumber==USERVO_X)
	{
		if(value>=USERVOMIN_X && value<=USERVOMAX_X)
		{
			adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
			currentPWMs[servoNumber]=value;
			return true;
		}
	}

	if(servoNumber==USERVO_Y)
	{
		if(value>=USERVOMIN_Y && value<=USERVOMAX_Y)
		{
			adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
			currentPWMs[servoNumber]=value;
			return true;
		}
	}

	if(servoNumber==ROTSERVO)
	{
		if(value>=ROTSERVO_MIN && value<=ROTSERVO_MAX)
		{
			adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
			currentPWMs[servoNumber]=value;
			return true;
		}
	}

	if(servoNumber==SHOULDERSERVORIGHT)
	{
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
	}

	if(servoNumber==ELBOWSERVO)
	{
		if(value>=ELBOWSERVO_MIN && value<=ELBOWSERVO_MAX)
		{
			adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
			currentPWMs[servoNumber]=value;
			return true;
		}
	}

	if(servoNumber==UNDERARMROTSERVO)
	{
		if(value>=UNDERARMROTSERVO_MIN && value<=UNDERARMROTSERVO_MAX)
		{
			adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
			currentPWMs[servoNumber]=value;
			return true;
		}
	}

	if(servoNumber==WRISTUPSERVO)
	{
		if(value>=WRISTUPSERVO_MIN && value<=WRISTUPSERVO_MAX)
		{
			adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
			currentPWMs[servoNumber]=value;
			return true;
		}
	}

	if(servoNumber==WRISTTWISTSERVO)
	{
		if(value>=WRISTTWISTSERVO_MIN && value<=WRISTTWISTSERVO_MAX)
		{
			adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
			currentPWMs[servoNumber]=value;
			return true;
		}
	}

	if(servoNumber==CLAWSERVO)
	{
		if(value>=CLAWSERVO_MIN && value<=CLAWSERVO_MAX)
		{
			adafruit_PWMServoDriver.setPWM(servoNumber, 0, value);
			currentPWMs[servoNumber]=value;
			return true;
		}
	}
	return false;
}

inline double getPulseFromAngle(const int servoNumber, const int angle)
{
	return 0; //TODO
}


void ServoFunctions::smoothMove(const int servoNumber, const int angle, const int speed)
{
	
	//TODO

}

void ServoFunctions::rsmoothMove_pulse(const int servoNumber, const int newPulse, const int speed, const int thread, const int task)
{
    if(smoothMove_pulse(servoNumber, newPulse,speed,thread,task)) resetTasks(thread);
}

bool ServoFunctions::smoothMove_pulse(const int servoNumber, const int newPulse, const int speed, const int thread, const int task)
{
    if((cycleClock%speed)==0)
    {
        if((!m_taskComplete[thread][task] && m_taskComplete[thread][task-1]))
        {
           
            if(abs(newPulse-currentPWMs[servoNumber])<3)
            {
                m_taskComplete[thread][task]=true;
                return true;
            }
            if(currentPWMs[servoNumber] <= newPulse)
            {
                setPWM(servoNumber, currentPWMs[servoNumber]+1);
            }else{
                setPWM(servoNumber, currentPWMs[servoNumber]-1);
            }
        }
        return false;
    }
}


void ServoFunctions::moveUSInCircle(const int speed)
{
    int xmin=USERVOMIN_X+110;
    int xmax=USERVOMAX_X-110;
    int ymin=USERVOMIN_Y+30;
    int ymax=USERVOMAX_Y-80;
    
    
    smoothMove_pulse(USERVO_X, xmin,speed,0,1);
    smoothMove_pulse(USERVO_X, xmax,speed,0,2);
    smoothMove_pulse(USERVO_Y, ymin,speed,0,3);
    smoothMove_pulse(USERVO_X, xmin,speed,0,4);
    smoothMove_pulse(USERVO_X, xmax,speed,0,5);
    rsmoothMove_pulse(USERVO_Y, ymax,speed,0,6);
    
    /*
    smoothMove_pulse(USERVO_Y, ymax,10,0,3);
    smoothMove_pulse(USERVO_X, xmin,100,0,1);
    smoothMove_pulse(USERVO_X, xmax,100,0,2);
    smoothMove_pulse(USERVO_X, xmin,100,0,3);
    
    smoothMove_pulse(USERVO_X, xmax,100,0,3);

    if(smoothMove_pulse(USERVO_Y, ymin,10,0,4))
    {
        resetTasks(0);
    }
     */
}

bool ServoFunctions::sleep(const long int cycles, const int thread, const int task)
{
    if((!m_taskComplete[thread][task] && m_taskComplete[thread][task-1]))
    {
        if(m_sleepValues[thread]++>cycles)
        {
            m_taskComplete[thread][task]=true;
            m_sleepValues[thread]=0;
            return true;
        }
    }
    return false;
}

/*
void ServoFunctions::moveUSLeftRight(int speed)
{
    for(int i = USERVOMIN_X+170; i< USERVOMAX_X-170; i=i+1)
    {
        setPWM(0, i);
        delay(100/speed);
    }
    
    
    for(int i = USERVOMAX_X-170; i> USERVOMIN_X+170; i=i-1)
    {
        setPWM(0, i);
        delay(100/speed);
    }


}
*/

/*
int ServoFunctions::obstacleAvoidDrive(struct pt* pt) {
  PT_BEGIN(pt);
  int OLDobstacleFound=0;
  for(;;)
  {
      if(obstacleFound==0)
      {
        MOTOR_GO_FORWARD;
      }
      if(obstacleFound==1)
      {
        if(getPWM(USERVO_X)>185)
        {
          MOTOR_GO_LEFT;
        }else
        {
          MOTOR_GO_RIGHT;
        }
      }
      OLDobstacleFound = obstacleFound;
      PT_YIELD(pt);
  }
  PT_END(pt);
}
*/
