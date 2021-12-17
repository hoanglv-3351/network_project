#ifndef _NOTICE_H_
#define _NOTICE_H_

#include<stdio.h>
#include "user.h"

#define MAX_LENGTH 100

typedef struct {
    char ID[MAX_LENGTH];
    char content[MAX_LENGTH];
    User receiveUser;
} Notice;

#endif
