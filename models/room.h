#ifndef _ROOM_H_
#define _ROOM_H_


#include<stdio.h>
#include "user.h"
#include "workspace.h"

#define MAX_USERS_IN_ROOM 20
typedef struct Room{
    int ID;
    char name[32];
    int user_id[MAX_USERS_IN_WSP];
    int num_of_users;
    struct Room *next;
} Room;



Room * readOneRoomData(char filename[], int wsp_id, int room_id);


Room * createNewRoom(int ID, char name[]);
void insertRoom(Room *root, char name[]);


Room *searchRoomByID(Room * root, int ID);
// input is a User, find all rooms user belong to
int * findRoomForUser(Room *root, int user_id, int * count);
void printAllRoom(Room *root);

#endif