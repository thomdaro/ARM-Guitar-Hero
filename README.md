Baremetal build of Guitar Hero in C with ARM architecture. Uses various peripherals to control the game.

Peripherals:

Display - 128 x 160 LCD screen used to display the game. Undraws and redraws various elements to create the illusion of movement. Uses DMA (direct memory access) to draw elements far more quickly than otherwise possible.
	
Guitar Hero controller - Main input device. Custom driver written to read input from guitar controller. Controller consists of an up-down "strum bar", five fret buttons, and a start button all used for input.
	
Gyroscope - Boards used for this project came with attached gyroscope. Shaking the board (or tilting) will pause the game.
	
SD card - stores audio files for playback.
	
Songs are generated via the use of .chart files, which consist of song specifications (title, artist, tempo) and a list of notes with tick-numbered timestamps. Chart files are read top to bottom, and each note is added into a queue where it is then drawn onscreen. When a note is played (by holding its corresponding fret button and strumming) or reaches the end of the display area, it is removed from the queue and undrawn.
	
Developed by Damien Thomas, Ben Staples and Dmitrii Galantsev for H335 - Honors Computer Structures at Indiana University Bloomington.
