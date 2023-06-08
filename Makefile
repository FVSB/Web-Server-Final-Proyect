CC = gcc
CFLAGS = -Wall -Wextra

all: test

test: server.o test.o
	$(CC) $(CFLAGS) -o test server.o test.o

server.o: server.c server.h
	$(CC) $(CFLAGS) -c server.c

test.o: test.c server.h
	$(CC) $(CFLAGS) -c test.c

clean:
	rm -f *.o test
