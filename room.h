#ifndef _ROOM_H_
#define _ROOM_H_

#define MAX_USERS_IN_ROOM 4

#include<stdio.h>
#include "user.h"
#include "workspace.h"

#define MAX_LENGTH 100

typedef struct {
    char ID[MAX_LENGTH];
    char name[MAX_LENGTH];
    User host;
    User users[MAX_USERS_IN_ROOM];
    WorkSpace onWSP;
} Room;

Room createRoom(User host, char* room_name);
void addUserToRoom(Room room, User user);

void out(Room room);

#endif
