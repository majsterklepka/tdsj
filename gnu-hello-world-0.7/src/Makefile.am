prefix=@prefix@
datarootdir=@datarootdir@
datadir=@datadir@
localedir = @localedir@
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

AM_CFLAGS=$(GTK_CFLAGS)
hello_LDADD=$(GTK_LIBS)

bin_PROGRAMS=hello
hello_SOURCES=main.c hello.h 
