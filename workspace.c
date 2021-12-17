#include "workspace.h"

#include<string.h>
#include<stdlib.h>

#include "user.h"

WorkSpace *headWSP = NULL;
WorkSpace *curWSP = NULL;

WorkSpace *updateListWSP() {
    headUser = updateListUser();
    WorkSpace workspace;
    int i = 0;
    FILE *f;
    //r == read file text
    if (!(f = fopen("db/workspaces.txt", "r"))){
        printf("\n File not found!! \n\n");
    }
    else{
        fscanf(f, "%d\n", &number_of_wsps);
        while (!feof(f)){
            workspace.host = (User *)malloc(sizeof(User));
            fscanf(f, "%d\n%d\n%s\n", &workspace.ID, &(workspace.host->ID), workspace.name);
            workspace.host = findUserByID(workspace.host->ID);

            WorkSpace *wspList = (WorkSpace *)malloc(sizeof(WorkSpace));
            strcpy(wspList->name, workspace.name);
            wspList->host = workspace.host;
            wspList->ID = workspace.ID;
            // wspList->users = workspace.users;
            wspList->next = headWSP;
            headWSP = wspList;
        }
    }
    fclose(f);
}

int isExistingWSP(char *input){
    int isExist = 0;
    WorkSpace *ptr;
    ptr = headWSP;
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
//     WorkSpace *wspList = updateListWSP();
//     WorkSpace *p = headWSP;
//     while(p != NULL) {
//         printf("%d --- %s --- %d\n", p->ID, p->name, p->host->ID);
//         p = p->next;
//     }
// }
