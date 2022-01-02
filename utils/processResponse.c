
#include "processResponse.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../models/workspace.h"
#include "../models/room.h"
#include "../models/user.h"

char *processResponseForViewWSP(User *user)
{

    static char information[512];
    int count = 0;
    WorkSpace *root = readWorkspaceData("db/workspaces.txt");
    int *list_wps = findWSPForUser(root, user->ID, &count);
    printf("Num Wsp of %s : %d\n", user->name, count);
    sprintf(information, "%d\n", count);
    if (count != 0)
    {
        for (int i = 0; i < count; i++)
        {
            {
                WorkSpace *wsp = searchWSPByID(root, list_wps[i]);
                char temp[3];
                sprintf(temp, "%d ", wsp->ID);
                strcat(information, temp);
                strcat(information, wsp->name);
                strcat(information, " ");

                if (wsp->host_id == user->ID)
                {
                    sprintf(temp, "%d\n", 1); // 1 is admin
                }
                else
                    sprintf(temp, "%d\n", 0);
                strcat(information, temp);
            }
        }
    }

    printf("Information for wsp list:  %s", information);
    return information;
}

char *processResponseForJoinWSP(User *user, int wsp_id)
{
    static char information[512];

    WorkSpace *wsp = readOneWSPData("db/workspaces.txt", wsp_id);
    User *root = readUserData("db/users.txt");
    char temp[3];
    strcat(information, wsp->name);
    strcat(information, "\n");
    
    
    //process rooms
    sprintf(temp, "%d\n", wsp->num_of_rooms);
    strcat(information, temp);
    for (int i = 0; i < wsp->num_of_rooms; i++)
    {
        
        sprintf(temp, "%d ", wsp->room_id[i]);
        strcat(information, temp);
        strcat(information, wsp->room_name[i]);
        strcat(information, "\n");
    }
    

    //process user
    sprintf(temp, "%d\n", wsp->num_of_users);
    strcat(information, temp);
    for (int i = 0; i < wsp->num_of_users; i++)
    {
        User *p = searchUserByID(root, wsp->user_id[i]);
        char temp[3];
        sprintf(temp, "%d ", p->ID);
        strcat(information, temp);
        strcat(information, p->name);
        strcat(information, " ");
    }
    

    printf("Information for wsp list:  %s", information);
    return information;
}