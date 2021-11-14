# Makefile for building our duplicates program.
PROJECT = duplicates
HEADERS = $(PROJECT).h
OBJ = duplicates.o list.o hashtable.o globals.o strSHA2.o options.o
C11 = cc -std=gnu99
CFLAGS = -Wall -pedantic -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)