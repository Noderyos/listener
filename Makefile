VERSION=2.0.0

INSTALL_PREFIX=/usr/local

CFLAGS+=-g -O2 -Wall -DVERSION=\"$(VERSION)\"

OBJSsl=pulse.o sl.o error.o utils.o
OBJSli=pulse.o listener.o error.o utils.o lib.o

all: setlistener listener

setlistener: $(OBJSsl)
	$(CC) -g -Wall -W $(OBJSsl) -ldl -lncurses -lpulse -lpulse-simple -o setlistener

listener: $(OBJSli)
	$(CC) -g -Wall -W $(OBJSli) -ldl -lm -lsndfile -lpulse -lpulse-simple -o listener -DCONFIGFILE=$(INSTALL_PREFIX)/etc/listener.conf

install: setlistener listener
	cp setlistener $(INSTALL_PREFIX)/bin
	cp listener $(INSTALL_PREFIX)/bin
	cp listener.conf $(INSTALL_PREFIX)/etc/

uninstall: clean
	rm -f $(INSTALL_PREFIX)/bin/setlistener
	rm -f $(INSTALL_PREFIX)/bin/listener
	rm -f $(INSTALL_PREFIX)/etc/listener.conf

clean:
	rm -f $(OBJSsl) $(OBJSli) $(OBJSsp) setlistener listener core

package: clean
	# source package
	rm -rf listener-$(VERSION)
	mkdir listener-$(VERSION)
	cp *.c *.h *.conf Makefile *.html license.txt listener-$(VERSION)
	tar cf - filter_lib_example* | (cd listener-$(VERSION) ; tar xf -)
	tar vczf listener-$(VERSION).tgz listener-$(VERSION)
	rm -rf listener-$(VERSION)
