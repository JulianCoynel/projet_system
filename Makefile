CC=clang

FICHIERS = shell

all: $(FICHIERS)

##############################################################################

%.o: %.c %.h
	$(CC) -c -g $<

shell.o: shell.c shell.h cp.h

cp.o : cp.c cp.h

#test_console.o: test_console.c console.h
###############################################################################
shell : shell.o cp.o
	$(CC) $^ -o $@


clear:
	rm -fR all *.o
