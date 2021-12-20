#include "user.h"

#include<string.h>
#include<stdlib.h>

void insertUser(User *user) {
    if(headUser == NULL) {
        headUser = user;
        curUser = user;
    }
    else {
        curUser->next = user;
        curUser = curUser->next;
        curUser->next = NULL;
    }
}

void updateListUser() {
    headUser = NULL;
    User user;
    int i = 0;
    FILE *f;
    //r == read file text
    if (!(f = fopen("db/users.txt", "r"))){
        printf("\n File not found!! \n\n");
    }
    else{
        fscanf(f, "%d\n", &number_of_users);
        if(number_of_users == 0) return;
        while (!feof(f)){
            fscanf(f, "%d\n%s\n%s\n", &(user.ID), user.name, user.password);
            user.isLogin = 0;

            User *userList = (User *)malloc(sizeof(User));
            strcpy(userList->name, user.name);
            strcpy(userList->password, user.password);
            userList->ID = user.ID;
            insertUser(userList);
        }
    }
    fclose(f);
}

int isExistingUser(char *name){
    int isExist = 0;
    User *ptr;
    ptr = headUser;
    while (ptr != NULL){
        if (strcmp(name, ptr->name) == 0) {
            isExist = 1;
            break;
        }
        ptr = ptr->next;
    }
    free(ptr);
    return isExist;
}

User *findUserByID(int ID){
    User *user = headUser;
    while (user != NULL){
        if (user->ID == ID)
            break;
        else 
            user = user->next;
    }
    return user;
}

// int main(){
//     User *userList = updateListUser();
//     User *p = headUser;
//     while(p != NULL) {
//         printf("%d --- %s --- %s\n", p->ID, p->name, p->password);
//         p = p->next;
//     }
//     User *kq = findUserByID(1);
//     if (kq != NULL) printf("%s", kq->name);
// }
