CC=gcc
glib-compile-resources=glib-compile-resources
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
CFLAGS+=-rdynamic
LIBS=$(shell pkg-config --libs gtk+-3.0)

all: bacade-gresource.c bacade-gresource.h
	$(CC) src/main.c src/print.c bacade-gresource.c -o bacade $(CFLAGS) $(LIBS)

bacade-gresource.c: bacade.gresource
	$(glib-compile-resources) --c-name bacade --generate-source --target bacade-gresource.c bacade.gresource.xml

bacade-gresource.h: bacade.gresource
	$(glib-compile-resources) --c-name bacade --generate-header --target bacade-gresource.h bacade.gresource.xml

bacade.gresource:
	$(glib-compile-resources) bacade.gresource.xml

.PHONY: clean
		
clean:
	rm -f bacade
	rm -f bacade-gresource.c
	rm -f bacade-gresource.h
	rm -f bacade.gresource

