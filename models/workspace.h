#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_


#define MAX_ROOM 10
#define MAX_USERS_IN_WSP 10
#define MAX_ROOMS_IN_WSP 5

#include<stdio.h>

#include "user.h"
#include "room.h"


#define atoa(x) #x

typedef struct WorkSpace{
    int ID;
    char name[32];
    
    int host_id;
    int user_id[MAX_USERS_IN_WSP];
    int room_id[MAX_ROOMS_IN_WSP];
    char room_name[MAX_ROOMS_IN_WSP][32];
    int num_of_rooms;
    int num_of_users;
    struct WorkSpace *next;
} WorkSpace;



WorkSpace * readWorkspaceData(char filename[]);
WorkSpace * readOneWSPData(char filename[], int wsp_id);


WorkSpace * createNewWSP(int id, int host_id, char  wsp_name[]);
void insertWSP(WorkSpace *root, int ID, int host_id, char name[]);
WorkSpace *searchWSPByID(WorkSpace * root, int ID);
void printAllWPS(WorkSpace *root);
void freeWorkspaceData(WorkSpace *root);


// input is a User, find all workspace user belong to
int * findWSPForUser(WorkSpace *root, int user_id, int * count);
// input is a user and a workspace, check if user belong to this wsp
char * checkWSPForUser(WorkSpace *root, int user_id, int * flag);
//input is a wsp and a room id/user id, check if this wsp contains the room/user
char *checkAvailableID(WorkSpace *wsp, int id, int *flag);


void addUserToWSP(WorkSpace workspace, User user);
//void updateMessage(WorkSpace workspace, Room room);

void kick(WorkSpace workspace, User user);
void join(WorkSpace workspace);

#endif