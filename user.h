#ifndef _USER_H_
#define _USER_H_

#include<stdio.h>

#define MAX_LENGTH 100

typedef struct {
    char ID[MAX_LENGTH];
    char name[MAX_LENGTH];
    char password[MAX_LENGTH];
} User;

#endif
