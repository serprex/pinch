all:pinch install
pinch:pinch.c
	gcc -std=gnu99 -O3 -s pinch.c -o pinch -fwhole-program -lpng
install:
	rm -f /usr/local/bin/pinch
	ln pinch /usr/local/bin