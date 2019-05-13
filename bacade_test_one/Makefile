CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0`

all:
	$(CC) src/main.c src/print.c bacade-gresource.c -o bacade $(CFLAGS) $(LIBS)
	
.PHONY: clean
		
clean:
	rm -f bacade
