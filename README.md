# VICTOR

ssh to victor:
   ssh victor@raspberrypi.local

VictorMaster contains the programs for the raspberry5 in victor. 

VictorSlave contains the programs for the arduino in victor


Arduino build commands:
	arduino-cli sketch new MyFirstSketch
	arduino-cli board list
	arduino-cli core list
	arduino-cli compile --fqbn arduino:avr:uno arduino
	arduino-cli upload -p /dev/cu.usbmodem14201 --fqbn arduino:avr:uno arduino
	arduino-cli monitor -p /dev/cu.usbmodem14201

Libraries
	--library arduino/lib/Arduino_AVRSTL
	--library arduino/lib/Adafruit_PWMServoDriver
	--library arduino/lib/ServoFunctions
	--library arduino/lib/Ultrasonic	
	--library arduino/lib/victorPrograms
	
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms


To clean:
	git clean -fxd

To build arduino:
	make build_arduino  (make)

To build and upload:
	make upload_arduino

To build, upload and monitor serial:
	make monitor_arduino













