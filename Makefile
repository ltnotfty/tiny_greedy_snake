TARGET=greedy_snake
CC=gcc
LDFLAGS= -lncurses
OBJS=main.o
OBJS += test.o

CFLAGS = -O2 -Wall -W -std=c99
DEBUG= -g -ggdb

ALL_EXE=${TARGET}
ALL_EXE+=test


${TARGET}: main.o
	$(CC) $< -o $@ $(LDFLAGS) $(DEBUG)
main.o: main.c
	$(CC) -c $(CFLAGS) $< $(DEBUG)
test: test.c
	$(CC) $< -o $@ $(LDFLAGS) $(DEBUG)
test.o: test.c
	$(CC) -c $(CFLAGS) $< $(DEBUG)
.PHONY: clean run 

clean: 
	-rm ${ALL_EXE} ${OBJS}
run:
	./${TARGET}	
