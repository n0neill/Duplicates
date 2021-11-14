# Makefile for building the duplicates program.
PROJECT = duplicates
HEADERS = $(PROJECT).h
OBJ = duplicates.o list.o hashtable.o globals.o strSHA2.o options.o
# use -std=gnu99 if using WSL
C11 = cc -std=c11
CFLAGS = -Wall -pedantic -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
