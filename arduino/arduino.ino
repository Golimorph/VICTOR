/*Victor robot program, created by Richard Edberg 2021.
 * 
 */

#include <ServoFunctions.h>
#include <stdio.h>
#include <stdbool.h>
#include <messageHandler.h>
#include <deque.h>
#include <uart.h>

ServoFunctions sf;
MessageHandler messageHandler(sf);
InverseKinematics inverseKinematics;

void setup() 
{
  Serial.begin(9600);
  sf.setup();
  Serial.println("Initialization successful!");
  sf.indInitCompleted();//nodd the US sensor to show init completed.
  while (Serial.available() > 0) 
  {
    Serial.read(); // Read and discard any incoming data to clean the buffer initially.
  }
}

void handleUartRequests()
{ 
  if(uart::uartMessageQueue.count() > 0)
  {
    std::vector<uint8_t> message = uart::uartMessageQueue.pop_front();
    messageHandler.handleMessage(message);
  }
}

void loop() 
{ 
  uart::checkInbox();
  handleUartRequests();
  sf.refresh();
}




/*
  if(millis() > 2000  && !step1Done)
  {
    //Serial.println("step1 done!----------------------------------");
    step1Done = true;
    sf.moveServo(ELBOWSERVO, 0, 2000);
    sf.moveServo(SHOULDERSERVORIGHT, 0, 2000);

    sf.moveServo(UNDERARMROTSERVO, 90, 2000);
    sf.moveServo(WRISTUPSERVO, 90, 2000);
    sf.moveServo(WRISTTWISTSERVO, -90, 2000);
  }

  if(millis() > 5000 && !step2Done)
  {
    //Serial.println("step2 done!----------------------------------");
    step2Done = true;
    sf.moveServo(ELBOWSERVO, 40, 2000);
    sf.moveServo(SHOULDERSERVORIGHT, 40, 2000);
    sf.moveServo(WRISTUPSERVO, 10, 2000);
  }

  if(millis() > 8000 && !step3Done)
  {
    //Serial.println("step3 done!----------------------------------");
    step3Done = true;
    sf.moveServo(CLAWSERVO, -500, 300);
  }

  if(millis() > 10000 && !step3p5Done)
  {
    //Serial.println("step3p5 done!----------------------------------");
    step3p5Done = true;
    sf.moveServo(ELBOWSERVO, 10, 2000);
    sf.moveServo(SHOULDERSERVORIGHT, -10, 2000);
    sf.moveServo(WRISTUPSERVO, 90, 2000);
  }
*/