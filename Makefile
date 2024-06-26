
RASPBERY_CPP_REPO = Repository
RASPBERRY_SRCS = $(RASPBERY_CPP_REPO)/main.cc $(RASPBERY_CPP_REPO)/I2C/src/victorI2cMaster.cc $(RASPBERY_CPP_REPO)/GPIO/src/lights.cc $(RASPBERY_CPP_REPO)/Socket/src/victorServer.cc
RASPBERRY_INCS = -I $(RASPBERY_CPP_REPO)/I2C/inc/ -I $(RASPBERY_CPP_REPO)/GPIO/inc/ -I $(RASPBERY_CPP_REPO)/Socket/inc/ -I $(RASPBERY_CPP_REPO)/


#build everything and upload it to victor
victorWithArduino:
	#upload to victor
	rsync -r arduino victor@raspberrypi.local:/home/victor/Arduino
	rsync -r arduino/if/ victor@raspberrypi.local:/home/victor/Repository
	rsync -r raspberry/* victor@raspberrypi.local:/home/victor/Repository
	#build raspberry program
	ssh victor@raspberrypi.local 'g++ $(RASPBERRY_INCS) -o $(RASPBERY_CPP_REPO)/main $(RASPBERRY_SRCS) -lwiringPi'
	#build arduino
	ssh victor@raspberrypi.local '/home/victor/Arduino/arduino-cli compile --fqbn arduino:avr:uno /home/victor/Arduino/arduino --library /home/victor/Arduino/arduino/lib/Arduino_AVRSTL --library /home/victor/Arduino/arduino/lib/Adafruit_PWMServoDriver --library /home/victor/Arduino/arduino/lib/ServoFunctions --library /home/victor/Arduino/arduino/lib/Ultrasonic --library /home/victor/Arduino/arduino/lib/victorPrograms --library /home/victor/Arduino/arduino/lib/I2cMessageHandler --library /home/victor/Arduino/arduino/lib/InverseKinematics --library /home/victor/Arduino/arduino/if --library /home/victor/Arduino/arduino/externalLib'
	ssh victor@raspberrypi.local '/home/victor/Arduino/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno /home/victor/Arduino/arduino'

#copy and upload C++ code to raspberrypi and arduino on victor
victorRaspberry:
	rsync -r raspberry/* victor@raspberrypi.local:/home/victor/Repository
	rsync -r arduino/if/ victor@raspberrypi.local:/home/victor/Repository
	ssh victor@raspberrypi.local 'g++ $(RASPBERRY_INCS) -o $(RASPBERY_CPP_REPO)/main $(RASPBERRY_SRCS) -lwiringPi'

victorArduino:
	rsync -r arduino victor@raspberrypi.local:/home/victor/Arduino
	ssh victor@raspberrypi.local '/home/victor/Arduino/arduino-cli compile --fqbn arduino:avr:uno /home/victor/Arduino/arduino --library /home/victor/Arduino/arduino/lib/Arduino_AVRSTL --library /home/victor/Arduino/arduino/lib/Adafruit_PWMServoDriver --library /home/victor/Arduino/arduino/lib/ServoFunctions --library /home/victor/Arduino/arduino/lib/Ultrasonic --library /home/victor/Arduino/arduino/lib/victorPrograms --library /home/victor/Arduino/arduino/lib/I2cMessageHandler --library /home/victor/Arduino/arduino/lib/InverseKinematics --library /home/victor/Arduino/arduino/if --library /home/victor/Arduino/arduino/externalLib'
	ssh victor@raspberrypi.local '/home/victor/Arduino/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno /home/victor/Arduino/arduino'


runVictor: 
	ssh victor@raspberrypi.local 'Repository/main'

DANMARK_SRCS = danmark/main.kt danmark/socketClient/VictorClient.kt danmark/gui/DanmarkGui.kt
DANMARK_JAR = danmark/Danmark.jar
DANMARK_DEPS = danmark/deps/jline-3.21.0.jar
.PHONY: danmark
danmark: $(DANMARK_SRCS)
	kotlinc -classpath $(DANMARK_DEPS) $(DANMARK_SRCS) -include-runtime -d $(DANMARK_JAR)
	
runDanmark: danmark
	java -jar $(DANMARK_JAR)