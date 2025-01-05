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