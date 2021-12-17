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

int isInWSP(User user, WorkSpace workspace) {
    int inWSP = 0;
    char wspID[5];
    sprintf(wspID, "%d", workspace.ID);
    int userID;

    int count = 0; // number of users in wsp

    FILE *f;
    //r == read file text
    char filename[MAX_LENGTH];
    strcpy(filename, "db/");
    strcat(filename, "workspace_users_");
    strcat(filename, wspID);
    strcat(filename, ".txt");
    if (!(f = fopen(filename, "r"))){
        printf("\n File not found!! \n\n");
    }
    else{
        while (!feof(f)){
            fscanf(f, "%d\n", &userID);
            if(user.ID == userID){
                inWSP = 1;
                return inWSP;
            }
        }
    }
    fclose(f);
    return inWSP;
}

void updateWSP(WorkSpace *workspace){
    char wspID[5];
    sprintf(wspID, "%d", workspace->ID);
    int userID;

    FILE *f;
    //r == read file text
    char filename[MAX_LENGTH];
    strcpy(filename, "db/");
    strcat(filename, "workspace_rooms_");
    strcat(filename, wspID);
    strcat(filename, ".txt");
    if (!(f = fopen(filename, "r"))){
        printf("\n File not found!! \n\n");
    }
    else{
        int count=0;
        while (!feof(f)){
            fscanf(f, "%d\n%s\n", &(workspace->rooms[count].ID), workspace->rooms[count].name);
            count++;
        }
        workspace->num_of_rooms = count;
    }
    fclose(f);

    // strcpy(filename, "");
    strcpy(filename, "db/");
    strcat(filename, "workspace_users_");
    strcat(filename, wspID);
    strcat(filename, ".txt");
    if (!(f = fopen(filename, "r"))){
        printf("\n File not found!! \n\n");
    }
    else{
        int count=0;
        while (!feof(f)){
            fscanf(f, "%d\n", &userID);
            count++;
        }
        workspace->num_of_users = count;
    }
}

int hasRoom(WorkSpace workspace, char *room_name) {
    int isHavingRoom = 0;
    int i;
    for(i=0 ; i<workspace.num_of_rooms ; i++) {
        if(strcmp(workspace.rooms[i].name, room_name)) {
            isHavingRoom = 1;
            return isHavingRoom;
        }
    }
    return isHavingRoom;
}

void signIn();
void signUp();

// int main(){
//     WorkSpace *wspList = updateListWSP();
//     WorkSpace *p = headWSP;
//     WorkSpace *test = headWSP->next;
//     while(p != NULL) {
//         printf("%d --- %s --- %d\n", p->ID, p->name, p->host->ID);
//         p = p->next;
//     }
//     User x = *headUser;
//     printf("%s\n", x.name);
//     if(isInWSP(x, *test)) {
//         printf("%s is in WSP!\n", x.name);
//     }
//     updateWSP(test);
//     printf("%d --- %s\n", test->rooms[0].ID, test->rooms[0].name);
//     printf("%d --- %s\n", test->rooms[1].ID, test->rooms[1].name);
//     printf("there are %d rooms and %d people in this wsp\n", test->num_of_rooms, test->num_of_users);
    
//     if(hasRoom(*test, "room2")) {
//         printf("Room 2 has been created in this WSP\n");
//     }
// }
