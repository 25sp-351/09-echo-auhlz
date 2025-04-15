all: main

OBJS = main.o connection.o server_socket.o
CC = gcc
CFLAGS = -Wall

main: $(OBJS)
	$(CC) -o main $(CFLAGS) $(OBJS) $(LDFLAGS)

connection.o: connection.c connection.h 

server_socket.o: server_socket.c server_socket.h

clean:
	rm -f main $(OBJS) connection.o server_socket.o
