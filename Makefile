#build everything and upload it to victor
victorWithArduino:
	#upload to victor
	rsync -r arduino victor@raspberrypi.local:/home/victor/Arduino
	rsync -r raspberry/* victor@raspberrypi.local:/home/victor/Repository
	#build raspberry program
	ssh victor@raspberrypi.local 'g++ -o Repository/server Repository/server.cpp -lwiringPi'
	#build arduino
	ssh victor@raspberrypi.local '/home/victor/Arduino/arduino-cli compile --fqbn arduino:avr:uno /home/victor/Arduino/arduino --library /home/victor/Arduino/arduino/lib/Arduino_AVRSTL --library /home/victor/Arduino/arduino/lib/Adafruit_PWMServoDriver --library /home/victor/Arduino/arduino/lib/ServoFunctions --library /home/victor/Arduino/arduino/lib/Ultrasonic --library /home/victor/Arduino/arduino/lib/victorPrograms'
	ssh victor@raspberrypi.local '/home/victor/Arduino/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno /home/victor/Arduino/arduino'

#copy and upload C++ code to raspberrypi and arduino on victor
victor:
	rsync -r raspberry/* victor@raspberrypi.local:/home/victor/Repository
	ssh victor@raspberrypi.local 'g++ -o Repository/server Repository/server.cpp -lwiringPi'

runVictor: 
	ssh victor@raspberrypi.local 'Repository/server'

DANMARK_SRCS = danmark/main.kt danmark/socketClient/VictorClient.kt danmark/gui/DanmarkGui.kt
DANMARK_JAR = danmark/Danmark.jar
DANMARK_DEPS = danmark/deps/jline-3.21.0.jar
.PHONY: danmark
danmark: $(DANMARK_SRCS)
	kotlinc -classpath $(DANMARK_DEPS) $(DANMARK_SRCS) -include-runtime -d $(DANMARK_JAR)
	
runDanmark: danmark
	java -jar $(DANMARK_JAR)


#DEBUGGING ONLY
build_usb_arduino:
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms
upload_usb_arduino:
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms
	arduino-cli upload -p /dev/cu.usbserial-1420 --fqbn arduino:avr:uno arduino
monitor_usb_arduino:
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms
	arduino-cli upload -p /dev/cu.usbserial-1420 --fqbn arduino:avr:uno arduino
	arduino-cli monitor -p /dev/cu.usbserial-1420