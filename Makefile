.PHONY: all clean install uninstall

PREFIX = /usr/local
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
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f lowbattery $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/lowbattery

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/lowbattery

