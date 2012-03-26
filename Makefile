all:pinch bitch install
pinch:pinch.c
	gcc -std=gnu99 -Os -s pinch.c -o pinch -fwhole-program -lpng
bitch:bitch.c
	gcc -std=gnu99 -Os -s bitch.c -o bitch -fwhole-program -lpng
install:
	rm -f /usr/local/bin/pinch /usr/local/bin/bitch
	ln bitch /usr/local/bin
	ln pinch /usr/local/bin