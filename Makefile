build_all:
	#raspberry program
	g++ -o raspberry/server raspberry/server.cpp
	#arduino program
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms
	#mac program
	kotlinc -classpath danmark/jline-3.21.0.jar danmark/Client.kt -include-runtime -d danmark/Client.jar

build_usb_arduino:
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms

upload_usb_arduino:
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms
	arduino-cli upload -p /dev/cu.usbserial-1420 --fqbn arduino:avr:uno arduino

monitor_usb_arduino:
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms
	arduino-cli upload -p /dev/cu.usbserial-1420 --fqbn arduino:avr:uno arduino
	arduino-cli monitor -p /dev/cu.usbserial-1420

kotlin:
	kotlinc -classpath danmark/jline-3.21.0.jar danmark/Client.kt -include-runtime -d danmark/Client.jar

krun:
	kotlin -classpath danmark/jline-3.21.0.jar:danmark/Client.jar ClientKt

#copy and upload C++ code to raspberrypi and arduino on victor
upload:
	scp -r arduino victor@raspberrypi.local:/home/victor/Arduino
	scp -r raspberry/* victor@raspberrypi.local:/home/victor/Repository
