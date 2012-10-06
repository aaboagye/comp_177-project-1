CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -D_POSIX_SOURCE

server.o:	server.c
	$(CC) $(CFLAGS) -c server.c

server:	server.o
	$(CC) $(CFLAGS) -o server server.o

clean:	server
	rm server server.o
