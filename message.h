#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include<stdio.h>
#include "user.h"
// #include "workspace.h"
// #include "room.c"

int number_of_messages = 0;

#define MAX_LENGTH 100

typedef struct Message{
    int ID;
    char content[MAX_LENGTH];
    User sendUser;
    User receiveUser;
    struct Message *parent;
    struct Message *next;
} Message;

Message *headMess = NULL;
Message *curMess = NULL;

#endif
