#ifndef _ROOM_H_
#define _ROOM_H_

#define MAX_USERS_IN_ROOM 4

#include<stdio.h>
#include "user.h"
#include "workspace.h"

#define MAX_LENGTH 100

typedef struct {
    int ID;
    char name[MAX_LENGTH];
    User users[MAX_USERS_IN_ROOM];
    struct WorkSpace *onWSP;
} Room;

Room createRoom(User host, char* room_name);
void addUserToRoom(Room room, User user);

void out(Room room);

#endif
