CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: server client

client: client.o utils.o user.o workspace.o message.o screen.o
	${CC} client.o utils.o user.o workspace.o message.o screen.o -o client -pthread -lm

server: server.o utils.o user.o workspace.o room.o message.o processResponse.o
	${CC} server.o utils.o user.o workspace.o room.o message.o processResponse.o -o server -pthread -lm


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
message.o: models/message.c
	${CC} ${CFLAGS} models/message.c


processResponse.o: utils/processResponse.c
	${CC} ${CFLAGS} utils/processResponse.c

screen.o: views/screen.c
	${CC} ${CFLAGS} views/screen.c



clean:
	rm -f *.o *~