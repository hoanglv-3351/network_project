#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include<stdio.h>
#include "user.h"

#define MAX_LENGTH 100

typedef struct {
    int ID;
    char content[MAX_LENGTH];
    User sendUser;
    User receiveUser;
    struct Message *parent;
} Message;

#endif