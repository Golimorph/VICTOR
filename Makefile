
RASPBERY_CPP_REPO = Repository
RASPBERRY_SRCS = $(RASPBERY_CPP_REPO)/main.cc $(RASPBERY_CPP_REPO)/Socket/src/victorServer.cc $(RASPBERY_CPP_REPO)/UART/src/victorUart.cc $(RASPBERY_CPP_REPO)/InverseKinematics/src/inverseKinematics.cc $(RASPBERY_CPP_REPO)/StateMachine/src/stateMachine.cc $(RASPBERY_CPP_REPO)/StateMachine/src/approach.cc $(RASPBERY_CPP_REPO)/Sensors/src/camera.cc
RASPBERRY_INCS = -I $(RASPBERY_CPP_REPO)/Socket/inc/ -I $(RASPBERY_CPP_REPO)/UART/inc/ -I $(RASPBERY_CPP_REPO)/InverseKinematics/inc/ -I $(RASPBERY_CPP_REPO)/StateMachine/inc/ -I $(RASPBERY_CPP_REPO)/Sensors/inc/ -I $(RASPBERY_CPP_REPO)/Utils/inc/ -I $(RASPBERY_CPP_REPO)/if -I $(RASPBERY_CPP_REPO)/


#build everything and upload it to victor
victorWithArduino:
	#stop the program execution on victor
	ssh victor@raspberrypi.local 'sudo systemctl stop victorProgram.service'
	#upload to victor
	rsync -r arduino victor@raspberrypi.local:/home/victor/Arduino
	rsync -r arduino/if/ victor@raspberrypi.local:/home/victor/Repository
	rsync -r raspberry/* victor@raspberrypi.local:/home/victor/Repository
	#build raspberry program
	ssh victor@raspberrypi.local 'g++ -std=c++20 $(RASPBERRY_INCS) -o $(RASPBERY_CPP_REPO)/main $(RASPBERRY_SRCS)'
	#build arduino
	ssh victor@raspberrypi.local '/home/victor/bin/arduino-cli compile --fqbn arduino:avr:uno /home/victor/Arduino/arduino --library /home/victor/Arduino/arduino/lib/Arduino_AVRSTL --library /home/victor/Arduino/arduino/lib/Adafruit_PWMServoDriver --library /home/victor/Arduino/arduino/lib/ServoFunctions --library /home/victor/Arduino/arduino/lib/Ultrasonic --library /home/victor/Arduino/arduino/lib/victorPrograms --library /home/victor/Arduino/arduino/lib/MessageHandler --library /home/victor/Arduino/arduino/lib/InverseKinematics --library /home/victor/Arduino/arduino/if --library /home/victor/Arduino/arduino/externalLib'
	ssh victor@raspberrypi.local '/home/victor/bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno /home/victor/Arduino/arduino'

#copy and upload C++ code to raspberrypi and arduino on victor
victorRaspberry:
	rsync -r raspberry/* victor@raspberrypi.local:/home/victor/Repository
	rsync -r arduino/if/ victor@raspberrypi.local:/home/victor/Repository
	ssh victor@raspberrypi.local 'g++ -std=c++20 $(RASPBERRY_INCS) -o $(RASPBERY_CPP_REPO)/main $(RASPBERRY_SRCS)'

victorArduino:
	rsync -r arduino victor@raspberrypi.local:/home/victor/Arduino
	ssh victor@raspberrypi.local '/home/victor/bin/arduino-cli compile --fqbn arduino:avr:uno /home/victor/Arduino/arduino --library /home/victor/Arduino/arduino/lib/Arduino_AVRSTL --library /home/victor/Arduino/arduino/lib/Adafruit_PWMServoDriver --library /home/victor/Arduino/arduino/lib/ServoFunctions --library /home/victor/Arduino/arduino/lib/Ultrasonic --library /home/victor/Arduino/arduino/lib/victorPrograms --library /home/victor/Arduino/arduino/lib/MessageHandler --library /home/victor/Arduino/arduino/lib/InverseKinematics --library /home/victor/Arduino/arduino/if --library /home/victor/Arduino/arduino/externalLib'
	ssh victor@raspberrypi.local '/home/victor/bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno /home/victor/Arduino/arduino'

DANMARK_SRCS = danmark/main.kt danmark/socketClient/VictorClient.kt danmark/gui/DanmarkGui.kt
DANMARK_JAR = danmark/Danmark.jar
DANMARK_DEPS = danmark/deps/jline-3.21.0.jar
.PHONY: danmark
danmark: $(DANMARK_SRCS)
	kotlinc -classpath $(DANMARK_DEPS) $(DANMARK_SRCS) -include-runtime -d $(DANMARK_JAR)
	
runDanmark: danmark
	java -jar $(DANMARK_JAR)

start:
	ssh victor@raspberrypi.local 'sudo systemctl start victorProgram.service'
	
stop:
	ssh victor@raspberrypi.local 'sudo systemctl stop victorProgram.service'

log:
	ssh victor@raspberrypi.local 'sudo journalctl -u victorProgram.service -f'

status:
	ssh victor@raspberrypi.local 'sudo systemctl status victorProgram.service'








