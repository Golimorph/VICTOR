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
	sudo systemctl start victorProgram.service
	sudo systemctl status victorProgram.service


