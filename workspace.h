#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#define MAX_LENGTH 100
#define MAX_ROOM 10
#define MAX_USERS_IN_WSP 50

#include<stdio.h>
#include "room.h"
#include "user.h"

typedef struct {
    char ID[MAX_LENGTH];
    char name[MAX_LENGTH];
    Room rooms[MAX_ROOM];
    User host;
    User users[MAX_USERS_IN_WSP];
} WorkSpace;

WorkSpace createWorkSpace(User host, char* wsp_name);
void addUserToWSP(WorkSpace workspace, User user);
void createRoom(WorkSpace workspace, char* room_name);

void kick(WorkSpace workspace, User user);
void join(WorkSpace workspace);

#endif
