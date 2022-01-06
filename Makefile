CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: server client

client: client.o utils.o screen.o
	${CC} client.o utils.o screen.o -o client -pthread -lm

server: server.o utils.o user.o workspace.o room.o message.o processResponse.o notice.o
	${CC} server.o utils.o user.o workspace.o room.o message.o processResponse.o notice.o -o server -pthread -lm


server.o: server.c
	${CC} ${CFLAGS} server.c -pthread
client.o: client.c
	${CC} ${CFLAGS} client.c
user.o: models/user.c
	${CC} ${CFLAGS} models/user.c
room.o: models/room.c
	${CC} ${CFLAGS} models/room.c
workspace.o: models/workspace.c
	${CC} ${CFLAGS} models/workspace.c
message.o: models/message.c
	${CC} ${CFLAGS} models/message.c
notice.o: models/notice.c
	${CC} ${CFLAGS} models/notice.c

utils.o: utils/utils.c
	${CC} ${CFLAGS} utils/utils.c
processResponse.o: utils/processResponse.c
	${CC} ${CFLAGS} utils/processResponse.c

screen.o: views/screen.c
	${CC} ${CFLAGS} views/screen.c



clean:
	rm -f *.o *~