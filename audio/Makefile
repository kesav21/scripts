CC=gcc
CFLAGS=-lm -D_REENTRANT -lpulse -pthread

bin/pulsetest: obj/main.o obj/callbacks.o obj/utils.o
	$(CC) $(CFLAGS) $^ -o $@

obj/main.o: src/main.c src/data.h src/callbacks.h
	$(CC) $< -c -o $@

obj/callbacks.o: src/callbacks.c src/data.h src/utils.h src/callbacks.h
	$(CC) $< -c -o $@

obj/utils.o: src/utils.c src/utils.h
	$(CC) $< -c -o $@

.PHONY: clean
clean:
	rm obj/*.o
