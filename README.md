Compile and upload everything:

	make

Uploading and compile raspberry or Arduino programs on victor:

	make victorRaspberry 
	make victorArduino

Compile danmark program: 

	make danmark

Run:

	make runVictor
	make runDanmark

To format code:
 	cstyle 

Check status on victor:
	sudo systemctl stop victorProgram.service
	sudo systemctl status victorProgram.service
	sudo journalctl -u victorProgram.service -f

	make start
	make stop
	make log
	make status


