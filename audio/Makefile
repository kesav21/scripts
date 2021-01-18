CFLAGS=-lm -D_REENTRANT -lpulse -pthread

all: bin/pulsetest

obj/utils.o: src/utils.c
	gcc src/utils.c -c -o obj/utils.o

obj/callbacks.o: src/callbacks.c src/data.h
	gcc src/callbacks.c -c -o obj/callbacks.o

obj/core.o: src/core.c src/data.h src/callbacks.h
	gcc src/core.c -c -o obj/core.o

obj/main.o: src/main.c src/data.h src/core.h
	gcc src/main.c -c -o obj/main.o

bin/pulsetest: obj/main.o obj/utils.o obj/callbacks.o obj/core.o
	gcc $(CFLAGS) obj/main.o obj/utils.o obj/callbacks.o obj/core.o -o bin/pulsetest
