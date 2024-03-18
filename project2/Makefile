# project2: multiprocessing
# csc 328 fall 2023
DEBUG = -g

EXE = project2

OBJ = multiprocessing.o

SRC = multiprocessing.c

$(EXE): $(OBJ) $(SRC)
	gcc -o $(EXE) $(OBJ) $(DEBUG)

multiprocessing.o: multiprocessing.c
	gcc -c multiprocessing.c $(DEBUG)

.PHONY: clean
clean:
	rm -f *o $(EXE)
all:
.PHONY: submit
submit:
	~schwesin/bin/submit csc328 project2
