.PHONY: all clean install uninstall

all: lowbattery

lowbattery:
	gcc -O2 `pkg-config --cflags --libs libnotify` lowbattery.c -o lowbattery

clean:
	rm lowbattery

install: all
	cp -f lowbattery /usr/local/bin
	chmod 755 /usr/local/bin/lowbattery

uninstall:
	rm -f /usr/local/bin/lowbattery

