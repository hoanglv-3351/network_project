#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_


#define MAX_ROOM 10
#define MAX_USERS_IN_WSP 10

#include<stdio.h>

#include "user.h"
#include "room.h"


#define atoa(x) #x

typedef struct WorkSpace{
    int ID;
    char name[32];
    
    int host_id;
    int user_id[MAX_USERS_IN_WSP];
    //Room *rooms[MAX_ROOM];
    int num_of_users;
    int num_of_rooms;
    struct WorkSpace *next;
} WorkSpace;



WorkSpace * readWorkspaceData(char filename[]);


WorkSpace * createNewWSP(int id, int host_id, char  wsp_name[]);
void insertWSP(WorkSpace *root, int ID, int host_id, char name[]);
WorkSpace *searchWSPByID(WorkSpace * root, int ID);
void printAllWPS(WorkSpace *root);

int * findWSPForUser(WorkSpace *root, int user_id, int * count);

void addUserToWSP(WorkSpace workspace, User user);
//void updateMessage(WorkSpace workspace, Room room);

void kick(WorkSpace workspace, User user);
void join(WorkSpace workspace);

#endif