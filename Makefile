CC=clang

FICHIERS = shell

all: $(FICHIERS)

##############################################################################

%.o: %.c %.h
	$(CC) -c -g $<

shell.o: shell.c shell.h

#test_console.o: test_console.c console.h
###############################################################################
shell : shell.o
	$(CC) $^ -o $@


clear:
	rm -fR all *.o
