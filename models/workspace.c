#include "user.h"
#include "room.h"
#include "workspace.h"
#include "signal.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

WorkSpace *createNewWSP(int id, int host_id, char wsp_name[])
{
    WorkSpace *new = (WorkSpace *)malloc(sizeof(WorkSpace));

    new->ID = id;
    new->host_id = host_id;
    strcpy(new->name, wsp_name);
    new->num_of_rooms = 0;
    new->num_of_users = 0;

    

    // read Each WorkSpace users
    char filename[32] = "db/workspace_users_";
    char tmp[2];
    sprintf(tmp, "%d", id);
    strcat(filename, tmp);
    strcat(filename, ".txt");
    FILE *f;
    if (!(f = fopen(filename, "r")))
    {
        //printf("\nCreate Workspace Database failed! File Users not found.\n");
        
    }
    else
    {
        while (!feof(f))
        {
            fscanf(f, "%d", &new->user_id[new->num_of_users++]);
        }
        fclose(f);
    }

    // read Each WorkSpace room
    strcpy(filename,"db/workspace_rooms_");
    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "%d", id);
    strcat(filename, tmp);
    strcat(filename, ".txt");
    printf("File room %s\n", filename);
    if (!(f = fopen(filename, "r")))
    {
        //printf("\nCreate Workspace Database failed! File Rooms not found.\n");
        
    }
    else
    {
        while (!feof(f))
        {
            fscanf(f, "%d\n", &new->room_id[new->num_of_rooms]);
            fscanf(f, "%[^\n]\n", new->room_name[new->num_of_rooms]);
            new->num_of_rooms += 1;
            if (feof(f))
            break;
        }
        fclose(f);
    }
    printf("Read data WSP %s have %d room, %d user\n", new->name, new->num_of_rooms, new->num_of_users );

    return new;
}
void insertWSP(WorkSpace *root, int ID, int host_id, char name[])
{
    WorkSpace *new = createNewWSP(ID, host_id, name);
    if (root == NULL)
    {
        new->next = root;
        root = new;
    }
    else
    {
        WorkSpace *p = root;
        //tro con tro toi cuoi danh sach lien ket
        while (p->next != NULL)
            p = p->next;
        p->next = new;
    }
}
void printAllWSP(WorkSpace *root)
{
    WorkSpace *p = root;
    while (p != NULL)
    {
        printf("Name %s\n", p->name);
        printf("Num of users %d\n", p->num_of_users);
        for (int i = 0; i < p->num_of_users; i++)
        {
            printf("%d ", p->user_id[i]);
        }
        p = p->next;
    }
}
WorkSpace *searchWSPByID(WorkSpace *root, int ID)
{
    WorkSpace *p = root;
    while (p != NULL)
    {
        if (p->ID == ID)
        {
            //WorkSpace *tmp = createNewWSP(p->ID, p->host_id, p->name);
            return p;
        }
        p = p->next;
    }
    return NULL;
}

WorkSpace *readWorkspaceData(char filename[])
{
    WorkSpace *root;

    int id, host_id;
    char wsp_name[32];

    FILE *f;

    //strcpy(filename, "db/users.txt")
    if (!(f = fopen(filename, "r")))
    {
        printf("Create Workspace Database failed! File not found.\n");
    }
    else
    {
        int number_of_workspaces = 0;
        int check = 1;
        fscanf(f, "%d\n", &number_of_workspaces);
        if (number_of_workspaces == 0)
            return NULL;
        while (!feof(f))
        {
            fscanf(f, "%d\n%d\n%s\n", &id, &host_id, wsp_name);

            if (check == 1)
            {
                root = createNewWSP(id, host_id, wsp_name);
                check = 0;
            }
            else
                insertWSP(root, id, host_id, wsp_name);
        }
    }
    fclose(f);
    return root;
}
// input wsp_id and read data of only this workspace
WorkSpace *readOneWSPData(char filename[], int wsp_id)
{
    WorkSpace *root;

    int id, host_id;
    char wsp_name[32];

    FILE *f;
    if (!(f = fopen(filename, "r")))
    {
        printf("Create Workspace Database failed! File not found.\n");
    }
    else
    {
        int number_of_workspaces = 0;
        fscanf(f, "%d\n", &number_of_workspaces);
        if (number_of_workspaces == 0)
            return NULL;
        while (!feof(f))
        {
            fscanf(f, "%d\n%d\n%s\n", &id, &host_id, wsp_name);

            if (id == wsp_id)
                root = createNewWSP(wsp_id, host_id, wsp_name);
        }
    }
    fclose(f);
    return root;
}
int valueInArray(int val, int *arr)
{

    for (int *i = arr; *i; i++)
    {
        if (*i == val)
            return 1;
    }
    return 0;
}

// input is a User, find all workspace user belong to
int * findWSPForUser(WorkSpace *root, int user_id, int *count)
{
    WorkSpace *p = root;
    static int list_wps[10];
    *count = 0;
    while (p != NULL)
    {
        if (valueInArray(user_id, p->user_id) == 1)
        {
            list_wps[*count] = p->ID;
            *count = *count + 1;
        }
        p = p->next;
    }

    return list_wps; // 0 is not exist
}

// input is a user and a workspace, check if user belong to this wsp
char *checkWSPForUser(WorkSpace *wsp, int user_id, int *flag)
{
    if (valueInArray(user_id, wsp->user_id) == 1)
    {
        *flag = 2;
        return MESS_JOIN_WSP_SUCCESS;
    }
    return MESS_JOIN_WSP_FAILED;
}

char *checkAvailableID(WorkSpace *wsp, int id, int *flag)
{
    if (valueInArray(id, wsp->user_id) == 1)
    {
        *flag = 3;
        return MESS_JOIN_ROOM_SUCCESS;
    }
    // if (valueInArray(id, wsp->room_id) == 1)
    // {
    //     *flag = 3;
    //     return MESS_JOIN_ROOM_SUCCESS;
    // }
    return MESS_INVALID_ID;

}

void addUserToWSP(WorkSpace workspace, User user)
{
}

void kick(WorkSpace workspace, User user)
{
}
void join(WorkSpace workspace)
{
}

// int main(int argc, char const *argv[])
// {

//     WorkSpace *root = readWorkspaceData("db/workspaces.txt");
// 	printAllWSP(root);
//     return 0;
// }

void freeWorkspaceData(WorkSpace *root)
{
    WorkSpace * current = root;
    WorkSpace * next;
 
   while (current != NULL)
   {
       next = current->next;
       free(current);
       current = next;
   }
   root = NULL;
}