CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: server client

client: client.o screen.o user.o
	${CC} client.o screen.o user.o -o client -pthread

server: server.o utils.o user.o workspace.o room.o screen.o
	${CC} server.o utils.o user.o workspace.o room.o screen.o -o server -pthread


server.o: server.c
	${CC} ${CFLAGS} server.c -pthread
client.o: client.c
	${CC} ${CFLAGS} client.c
utils.o: models/utils.c
	${CC} ${CFLAGS} models/utils.c
user.o: models/user.c
	${CC} ${CFLAGS} models/user.c
room.o: models/room.c
	${CC} ${CFLAGS} models/room.c
workspace.o: models/workspace.c
	${CC} ${CFLAGS} models/workspace.c

screen.o: views/screen.c
	${CC} ${CFLAGS} views/screen.c



clean:
	rm -f *.o *~