
#port=${arduino-cli board list | grep USB | awk '{print $1;}'}
#echo ${port}

build_arduino:
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms

upload_arduino:
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms
	arduino-cli upload -p /dev/cu.usbserial-1420 --fqbn arduino:avr:uno arduino

monitor_arduino:
	arduino-cli compile --fqbn arduino:avr:uno arduino --library arduino/lib/Arduino_AVRSTL --library arduino/lib/Adafruit_PWMServoDriver --library arduino/lib/ServoFunctions --library arduino/lib/Ultrasonic --library arduino/lib/victorPrograms
	arduino-cli upload -p /dev/cu.usbserial-1420 --fqbn arduino:avr:uno arduino
	arduino-cli monitor -p /dev/cu.usbserial-1420