CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -D_POSIX_SOURCE

client.o:	main.c
	$(CC) $(CFLAGS) -c main.c -o client.o

client:	client.o
	$(CC) $(CFLAGS) -o client client.o

server.o:	server.c
	$(CC) $(CFLAGS) -c server.c

server:	server.o
	$(CC) $(CFLAGS) -o server server.o

clean:	server
	rm server server.o
