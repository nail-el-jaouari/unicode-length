CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -fsanitize=address,bounds,undefined,leak
LDFLAGS = -fsanitize=address,bounds,undefined,leak

main.o: main.c unicode-length-utf8.h vec-str.h
	$(CC) $(CFLAGS) -c $< -o $@

unicode-length-utf8.o: unicode-length-utf8.c unicode-length-utf8.h
	$(CC) $(CFLAGS) -c $< -o $@

vec-str.o: vec-str.c vec-str.h
	$(CC) $(CFLAGS) -c $< -o $@

uc-length: main.o unicode-length-utf8.o vec-str.o
	$(CC) $(LDFLAGS) $^ -o $@

debug: main.c unicode-length-utf8.c vec-str.c
	$(CC) $(CFLAGS) -g -O0 $^ -o $@

clean:
	rm -f *.o debug uc-length