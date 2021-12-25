#include "user.h"
#include "room.h"
#include "workspace.h"

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>



WorkSpace * createNewWorkspace(int id, int host_id, char  wsp_name[]){
    WorkSpace * new =  (WorkSpace *)malloc(sizeof(WorkSpace));

    new->ID = id;
    new->host_id = host_id;
    strcpy(new->name, wsp_name);
    new->num_of_rooms = 0;
    new->num_of_users = 1; // always has 1 user : host

    // read Each WorkSpace users
    char filename[] = "workspace_users_";
    strcat(filename, atoa(id));
    FILE *f;
    if (!(f = fopen(filename,  "r")))
    {
        printf("\nCreate Workspace Database failed! File Users not found.\n\n");
    }
    else
    {
        while (!feof(f)){
            fscanf(f, "%d", &new->user_id[new->num_of_users++]);
        }
    }

    return new;
}
void insertWorkspace(User *root, int ID, int host_id, char name[])
{
  User * new = createNewWorkspace(ID, host_id, name);
  if(root==NULL)
    {
      new->next=root;
      root=new;
    }
  else
    {
      User *p=root;
      //tro con tro toi cuoi danh sach lien ket
      while(p->next!=NULL) p=p->next; 
      p->next=new;
    }
}


WorkSpace * readWorkspaceData(char filename[]){
    WorkSpace *root;

    int id, host_id;
    char wsp_name[32];
    
    
    FILE *f;

    //strcpy(filename, "db/users.txt")
    if (!(f = fopen(filename,  "r")))
    {
        printf("\nCreate Workspace Database failed! File not found.\n\n");
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
                root = createNewWorkspace(id, host_id,wsp_name);
                check = 0;
            }
            else
                insertWorkspace(root, id, host_id, wsp_name);
        }
    }
    fclose(f);
    return root;
}
int valueInArray(int val, int arr[])
{
    int i;
    for(i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        if(arr[i] == val)
            return 1;
    }
    return 0;
}

// input is a User, find all workspace user belong to
int * findWorkSpaceForUser(WorkSpace *root, User * u)
{
    int user_id = u->ID;
    WorkSpace *p =root;
    int list_wps[10];
    int count = 0;
    while (p != NULL)
    {
        if (valueInArray(user_id, p->user_id) == 1)
        {
            list_wps[count++] = p->ID;
        } 
        p = p->next;
    }
    
    return list_wps; // 0 is not exist
}

void addUserToWSP(WorkSpace workspace, User user){

}
void updateMessage(WorkSpace workspace, Room room){

}

void kick(WorkSpace workspace, User user){

}
void join(WorkSpace workspace){

}
