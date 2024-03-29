/*Victor robot program, created by Richard Edberg 2021-09-05.
 * 
 */
#include <Wire.h>
#include <ServoFunctions.h>
#include "../victorPrograms/victorPrograms.h"
#include<stdio.h>
#include<stdbool.h>
ServoFunctions sf(10,20);
VictorPrograms vp(&sf);




//I2C receive slave
void initI2C()
{
  Wire.begin(9); //I2C slave
  Wire.onReceive(receiveEvent);// I2C slave
}
int USdistance=0;
void receiveEvent(int bytes)
{
  USdistance = Wire.read();    // read one character from the I2C
}
//end I2C receive slave



void setup() 
{
initI2C();
sf.setup();
Serial.begin(9600); //terminal print
Serial.println("starting");
delay(1000);
}





int backCounter=0;
void loop() 
{ 
  sf.cycleClock++;
  //sf.updateMotorsHallSensorPositionBlocking();
 
  sf.moveUSInCircle(5);
  vp.obstacleAvoidDrive_0(USdistance);
   

/*
   sf.smoothMove_pulse(CLAWSERVO, CLAWSERVO_MAX, 20,3,1);
   sf.rsmoothMove_pulse(CLAWSERVO, CLAWSERVO_MIN+40, 20,3,2);
   
   sf.smoothMove_pulse(ROTSERVO, ROTSERVO_MAX, 20,4,1);
   sf.rsmoothMove_pulse(ROTSERVO, ROTSERVO_MIN, 20,4,2);
   
   sf.smoothMove_pulse(SHOULDERSERVORIGHT, SHOULDERSERVORIGHT_MID, 20,5,1);
   sf.rsmoothMove_pulse(SHOULDERSERVORIGHT, SHOULDERSERVORIGHT_MIN, 20,5,2);
   
   sf.smoothMove_pulse(ELBOWSERVO, ELBOWSERVO_MIN, 20,6,1);
   sf.rsmoothMove_pulse(ELBOWSERVO, ELBOWSERVO_MAX, 20,6,2);
  
   sf.smoothMove_pulse(WRISTUPSERVO, WRISTUPSERVO_MIN, 20,7,1);
   sf.rsmoothMove_pulse(WRISTUPSERVO, WRISTUPSERVO_MAX, 20,7,2);
   */
   

  //Serial.println(USdistance);
 }



/*
   sf.smoothMove_pulse(CLAWSERVO, CLAWSERVO_MAX, 20,1,1);
   if(sf.smoothMove_pulse(CLAWSERVO, CLAWSERVO_MIN+40, 20,1,2))
   {
    sf.resetTasks(1);
   }
   sf.smoothMove_pulse(ROTSERVO, ROTSERVO_MAX, 20,2,1);
   if(sf.smoothMove_pulse(ROTSERVO, ROTSERVO_MIN, 20,2,2))
   {
    sf.resetTasks(2);
   }
   sf.smoothMove_pulse(SHOULDERSERVORIGHT, SHOULDERSERVORIGHT_MID, 20,3,1);
   if(sf.smoothMove_pulse(SHOULDERSERVORIGHT, SHOULDERSERVORIGHT_MIN, 20,3,2))
   {
    sf.resetTasks(3);
   }
   sf.smoothMove_pulse(ELBOWSERVO, ELBOWSERVO_MIN, 20,3,1);
   if(sf.smoothMove_pulse(ELBOWSERVO, ELBOWSERVO_MAX, 20,3,2))
   {
    sf.resetTasks(3);
   }
   sf.smoothMove_pulse(WRISTUPSERVO, WRISTUPSERVO_MIN, 20,4,1);
   if(sf.smoothMove_pulse(WRISTUPSERVO, WRISTUPSERVO_MAX, 20,4,2))
   {
    sf.resetTasks(4);
   }
   sf.smoothMove_pulse(UNDERARMROTSERVO, UNDERARMROTSERVO_MIN, 20,5,1);
   if(sf.smoothMove_pulse(UNDERARMROTSERVO, UNDERARMROTSERVO_MAX, 20,5,2))
   {
    sf.resetTasks(5);
   }
    sf.smoothMove_pulse(WRISTTWISTSERVO, WRISTTWISTSERVO_MIN, 20,6,1);
   if(sf.smoothMove_pulse(WRISTTWISTSERVO, WRISTTWISTSERVO_MAX, 20,6,2))
   {
    sf.resetTasks(6);
   }
*/
  
