#ifndef _USER_H_
#define _USER_H_

#include<stdio.h>

#define MAX_LENGTH 100

int number_of_users = 0;    

typedef struct User{
    int ID;
    char name[MAX_LENGTH];
    char password[MAX_LENGTH];
    int isLogin;
    struct User *next;
} User;

User *headUser = NULL;
User *cur = NULL;

void updateListUser();
User findUserByID(int ID);

#endif
