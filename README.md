Compile and upload everything:

	make

Uploading and compile raspberry programs on victor:

	make victorRaspberry 

Compile danmark program: 

	make danmark

Run:

	make runVictor
	make runDanmark

Check status on victor:
	sudo systemctl start victorProgram.service
	sudo systemctl status victorProgram.service