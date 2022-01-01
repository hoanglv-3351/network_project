#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdio.h>
#include <time.h>

#include "user.h"
#include "workspace.h"
#include "signal.h"
// #include "room.c"

#define MESS_LEN 2048

// typedef struct Message{
//     int ID;
//     char content[MAX_LENGTH];
//     char datetime[64];
//     User *sendUser;
//     User *receiveUser;
//     struct Message *parent;
//     struct Message *next;
// } Message;

typedef struct Message{
    int ID;
    char content[MESS_LEN];
    time_t datetime;
    int send_id;
    int parent_id;
    //struct Message *parent;
    struct Message *next;
} Message;

Message * readMessData(char filename[]);
void writeMessData(Message *root, int wsp_id, int room_id);
char * createMessFilename(int wsp_id,int room_id);



Message * createNewMess( int parent_id,char time[],int send_id,  char content[]);
Message * insertMess(Message * root,  int parent_id,char time[],int send_id, char content[]);
void printAllMess(Message * root);
void freeMessData(Message * root);


Message *findMessByID(Message *root, int ID);
Message *findMessFromDate(time_t time);
Message *findMessFromToDate(time_t from_time, time_t to_time);
Message *findMessInDate(time_t time);

#endif