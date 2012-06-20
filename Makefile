all:bitch pinch synch
bitch:bitch.c ch.h
	gcc -std=gnu1x -Os -s -fwhole-program -lpng -pipe bitch.c -o bitch
pinch:pinch.c ch.h
	gcc -std=gnu1x -Os -s -fwhole-program -lpng -pipe pinch.c -o pinch
synch:synch.c ch.h
	gcc -std=gnu1x -Os -s -fwhole-program -lpng -pipe synch.c -o synch
stest:stest.c sgen.c sgen.h
	make && ./synch c a b r && gcc -std=gnu1x -g -pipe sgen.c stest.c -o stest -lglfw -lGL&&./stest
install:
	rm -f /usr/local/bin/bitch /usr/local/bin/pinch /usr/local/bin/synch
	ln bitch /usr/local/bin
	ln pinch /usr/local/bin
	ln synch /usr/local/bin
uninstall:
	rm -f /usr/local/bin/bitch /usr/local/bin/pinch /usr/local/bin/synch