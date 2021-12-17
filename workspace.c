#include "user.h"
#include "room.h"
#include "workspace.h"
#include "message.h"

#include<string.h>
#include<stdlib.h>

WorkSpace *headWSP = NULL;
WorkSpace *curWSP = NULL;

void updateListWSP() {
    updateListUser();
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
            fscanf(f, "%d\n%d\n%s\n", &workspace.ID, &(workspace.host.ID), workspace.name);
            workspace.host = findUserByID(workspace.host.ID);

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
            // workspace->rooms[count] = malloc(sizeof(Room));
            fscanf(f, "%d\n%s\n", &(workspace->rooms[count].ID), workspace->rooms[count].name);
            count++;
        }
        workspace->num_of_rooms = count;
    }
    fclose(f);

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

void updateMessage(WorkSpace workspace, Room room) {
    Message message;
    int count = 0;
    FILE *f;

    char wspID[5], roomID[5];
    char filename[MAX_LENGTH];
    sprintf(wspID, "%d", workspace.ID);
    sprintf(roomID, "%d", room.ID);

    strcpy(filename, "db/");
    strcat(filename, "mess_");
    strcat(filename, wspID);
    strcat(filename, "_");
    strcat(filename, roomID);
    strcat(filename, ".txt");

    //r == read file text
    if (!(f = fopen(filename, "r"))){
        printf("\n File not found!! \n\n");
    }
    else{
        fscanf(f, "%d\n", &number_of_messages);
        while (!feof(f)){
            message.parent = (Message *)malloc(sizeof(Message));
            fscanf(f, "%d\n%d\n%d\n%d\n", &(message.ID), &(message.parent->ID), &(message.sendUser.ID), &(message.receiveUser.ID));
            fgets(message.content, MAX_LENGTH, f);
            if(message.content[strlen(message.content) - 1] == '\n') {
                message.content[strlen(message.content) - 1] = '\0';
            }
            
            Message *messList = (Message *)malloc(sizeof(Message));
            strcpy(messList->content, message.content);  
       
            messList->ID = message.ID;
            messList->parent = (Message *)malloc(sizeof(Message));
            messList->parent->ID = message.parent->ID;
            messList->sendUser = findUserByID(message.sendUser.ID);
            messList->receiveUser = findUserByID(message.receiveUser.ID);

            messList->next = headMess;
            headMess = messList;
        }
    }
    fclose(f);
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
//     updateListWSP();
//     WorkSpace *p = headWSP;
//     WorkSpace *test = headWSP->next;
//     while(p != NULL) {
//         printf("%d --- %s --- %s\n", p->ID, p->name, p->host.name);
//         p = p->next;
//     }
//     User *x = headUser;

//     while(x!= NULL) {
//         printf("Name: %s\n", x->name);
//         x= x->next;
//     }

//     x = headUser;
//     if(isInWSP(*x, *test)) {
//         printf("%s is in WSP!\n", x->name);
//     }
//     updateWSP(test);
//     printf("%d --- %s\n", test->rooms[0].ID, test->rooms[0].name);
//     printf("%d --- %s\n", test->rooms[1].ID, test->rooms[1].name);
//     printf("there are %d rooms and %d people in this wsp\n", test->num_of_rooms, test->num_of_users);
    
//     if(hasRoom(*test, "room2")) {
//         printf("Room 2 has been created in this WSP\n");
//     }

//     updateMessage(*test, test->rooms[1]);

//     Message *mess = headMess;
//     while(mess != NULL) {
//         printf("%d - %s\n", mess->ID, mess->content);
//         mess = mess->next;
//     }
// }
