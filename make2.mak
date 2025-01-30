CC = gcc
CFLAGS = -Wall -g

all: client server

client: Client.c functions_2.c
	$(CC) $(CFLAGS) -o client Client.c functions_2.c

server: Server.c functions_1.c
	$(CC) $(CFLAGS) -o server Server.c functions_1.c

clean:
	rm -f client server