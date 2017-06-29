all: main.o func.o
		gcc main.o func.o -o text_editor -lcurses -g

main.o: main.c 
func.o: func.c
func.c: func.h

.PHONY: clean

clean:
		rm -rf *.o text_editor

