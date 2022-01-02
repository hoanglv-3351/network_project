
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
    int *list_wps = findWSPForUser(root, user->ID,&count);
    printf("Num Wsp of %s : %d\n", user->name,count);
    sprintf(information,"%d\n", count);
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
                    sprintf(temp, "%d\n",1); // 1 is admin
                }
                else
                    sprintf(temp, "%d\n",0);
                strcat(information, temp);
                
                
            }
        }
    }
    printf("Information for wsp list:  %s", information);
    return information;
}