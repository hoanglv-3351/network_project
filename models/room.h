#ifndef _ROOM_H_
#define _ROOM_H_


#include<stdio.h>
#include "user.h"
#include "workspace.h"

typedef struct Room{
    int ID;
    char name[32];
    User users[4];
    struct WorkSpace *onWSP;
} Room;

Room * createRoom(User host, char* room_name);
void addUserToRoom(Room room, User user);

void out(Room room);

#endif