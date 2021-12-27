#ifndef _USER_H_
#define _USER_H_

#include<stdio.h>

#define MAX_LENGTH 32

//int number_of_users = 0;    

typedef struct User{
    int ID;
    char name[MAX_LENGTH];
    char password[MAX_LENGTH];
    struct User *next;
} User;


User * readUserFile(char filename[]);


User * createNewUser(int ID, char username[], char password[]);
void insertUser(User * root, int ID, char username[], char password[]);
void printAllUser(User * root);
void freeUserData(User * root);

User * searchUserByUsername(User * root, char name[]);
User * searchUserByID(User * root, int ID);



char * verifyAccount(User * root, char *name, char *password, int  * flag);

#endif