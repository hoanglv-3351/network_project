#include "user.h"

#include<string.h>
#include<stdlib.h>

User *head = NULL;
User *cur = NULL;

void InsertFirst(User user){
    User *userList = (User *)malloc(sizeof(User));
    strcpy(userList->name, user.name);
    strcpy(userList->password, user.password);
    userList->next = head;
    head = userList;
}

void updateListUser(User *listUser) {
    User user;
    int i = 0;
    FILE *f;
    //r == read file text
    if (!(f = fopen("db/users.txt", "r"))){
        printf("\n File not found!! \n\n");
    }
    else{
        fscanf(f, "%d\n", &number_of_users);
        printf("%d\n", number_of_users);
        while (!feof(f)){
            fscanf(f, "%d\n%s\n%s", &user.ID, user.name, user.password);
            user.isLogin = 0;
            InsertFirst(user);
        }
    }
    fclose(f);
}

int isExisting(char *input){
    int isExist = 0;
    User *ptr;
    ptr = head;
    while (ptr != NULL){
        if (strcmp(input, ptr->name) == 0)
            isExist = 1;
        ptr = ptr->next;
    }
    return isExist;
}

void signIn();
void signUp();

// int main(){
//     User *userList;
//     updateListUser(userList);
//     User *p = head;
//     while(p != NULL) {
//         printf("%s --- %s\n", p->name, p->password);
//         p = p->next;
//     }
// }
