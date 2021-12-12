.PHONY: all clean install uninstall

SRC = lowbattery.c
OBJ = $(SRC:.c=.o)

all: lowbattery

.c.o:
	gcc -O2 `pkg-config --cflags libnotify`-c $<

lowbattery: lowbattery.o
	gcc `pkg-config --libs libnotify` lowbattery.o -o lowbattery

clean:
	rm -f lowbattery $(OBJ)

install: all
	cp -f lowbattery /usr/local/bin
	chmod 755 /usr/local/bin/lowbattery

uninstall:
	rm -f /usr/local/bin/lowbattery

