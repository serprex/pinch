all:pinch install
pinch:pinch.c
	gcc -std=gnu99 -O3 -s pinch.c -o pinch -fwhole-program -lpng
install:
	ln -f pinch /usr/local/bin