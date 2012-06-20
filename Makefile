all:pinch bitch install
pinch:pinch.c ch.h
	gcc -std=gnu1x -Os -s pinch.c -o pinch -fwhole-program -lpng -pipe
bitch:bitch.c ch.h
	gcc -std=gnu1x -Os -s bitch.c -o bitch -fwhole-program -lpng -pipe
install:
	rm -f /usr/local/bin/pinch /usr/local/bin/bitch
	ln bitch /usr/local/bin
	ln pinch /usr/local/bin