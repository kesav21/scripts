CC=gcc
CFLAGS=-lm -D_REENTRANT -lpulse -pthread

all: bin/pulsetest

obj/utils.o: src/utils.c src/utils.h
	$(CC) $< -c -o $@

obj/callbacks.o: src/callbacks.c src/data.h src/utils.h src/callbacks.h
	$(CC) $< -c -o $@

obj/core.o: src/core.c src/data.h src/callbacks.h src/core.h
	$(CC) $< -c -o $@

obj/main.o: src/main.c src/data.h src/core.h
	$(CC) $< -c -o $@

bin/pulsetest: obj/main.o obj/utils.o obj/callbacks.o obj/core.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm obj/*.o
