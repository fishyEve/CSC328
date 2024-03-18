# Eve Collier
# project3: threads
# csc 328 section 010 fall 2023
# Makefile

DEBUG = -g

EXE = project3

OBJ = threads.o

SRC = threads.c

$(EXE): $(OBJ) $(SRC)
	gcc -o $(EXE) $(OBJ) -lpthread $(DEBUG)

threads.o: threads.c
	gcc -c threads.c -lpthread $(DEBUG)

.PHONY: clean
clean:
	rm -f *o $(EXE)

all:

.PHONY: submit
submit:
	~schwesin/bin/submit csc328 project3
