
#include "processResponse.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../models/workspace.h"
#include "../models/room.h"
#include "../models/user.h"
#include "../models/message.h"
#include "utils.h"

char *processResponseForViewWSP(User *user, int size)
{

    // static char information[512];
    char * information = (char*) malloc(size * sizeof(char));
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

    printf("Information for wsp list: \n %s\n", information);
    return information;
}

char *processResponseForJoinWSP(User *user, int wsp_id, int size)
{
    // static char information[512];
    char * information = (char*) malloc(size * sizeof(char));

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

char *processResponseForJoinRoom(User *user, int wsp_id, int room_id, int size)
{
    // static char information[4096];
    char * information = (char*) malloc(size *sizeof(char));

    char filename[32];
    strcpy(filename, createMessFilename(wsp_id, room_id));
    Message *root = readMessData(filename);
    if (root == NULL)
    {
        return MESS_BEGIN_CHAT;
    }
    User *u_root = readUserData("db/users.txt");
    Message *p = root;

    int count_mess = 0;
    while (p != NULL)
    {
        count_mess += 1;
        p = p->next;
    }

    p = root;
    char temp[3];
    printf("Number of mess: %d\n", count_mess);
    while (p != NULL)
    {
        if (count_mess - 10 > 0)
        {
            count_mess--;
            p = p->next;
            continue;
        }
        sprintf(temp, "%d\n", p->ID);
        strcat(information, temp);

        char timestr[64];
        strcpy(timestr, convertTimeTtoString(p->datetime, 1));
        strcat(information, timestr);
        strcat(information, "\n");
        if (p->send_id != user->ID)
        {
            User *tmp = searchUserByID(u_root, p->send_id);
            strcat(information, tmp->name);
            strcat(information, "\n");
        }
        else
        {
            strcat(information, "0\n");
        }
        if (p->parent_id != 0)
        {
            Message *tmp = findMessByID(root, p->parent_id);
            strcat(information, tmp->content);
            strcat(information, "\n");
        }
        else
        {
            strcat(information, "0\n");
        }
        strcat(information, p->content);
        strcat(information, "\n");
        p = p->next;
    }

    // printf("Information message:\n  %s\n", information);
    return information;
}

char *processResponseForFindDate(User *user, int wsp_id, int room_id, char time[], int size)
{
    // static char information[4096];
    char * information = (char*) malloc(size * sizeof(char));

    char filename[32];
    strcpy(filename, createMessFilename(wsp_id, room_id));
    Message *root = readMessData(filename);
    User *u_root = readUserData("db/users.txt");
    Message *p = root;

    int count_mess = 0;

    p = root;
    char temp[3];
    while (p != NULL)
    {
        time_t time_ = convertStringToTimeT(time);
        if (difftime(p->datetime, time_) >= 0 )
        {
            if (count_mess == 10)
            {
                break;
            }
            count_mess++;
            sprintf(temp, "%d\n", p->ID);
            strcat(information, temp);

            char timestr[64];
            strcpy(timestr, convertTimeTtoString(p->datetime, 1));
            strcat(information, timestr);
            strcat(information, "\n");
            if (p->send_id != user->ID)
            {
                User *tmp = searchUserByID(u_root, p->send_id);
                strcat(information, tmp->name);
                strcat(information, "\n");
            }
            else
            {
                strcat(information, "0\n");
            }
            if (p->parent_id != 0)
            {
                Message *tmp = findMessByID(root, p->parent_id);
                strcat(information, tmp->content);
                strcat(information, "\n");
            }
            else
            {
                strcat(information, "0\n");
            }
            strcat(information, p->content);
            strcat(information, "\n");
        }
        p = p->next;
    }

    if (count_mess == 0)
    {
        strcpy(information, MESS_FIND_ERROR);
    }
    // printf("Information message:\n  %s\n", information);
    return information;
}


char *processResponseForFindContent(User *user, int wsp_id, int room_id, char content[], int size)
{
    //static char information[4096];
    char * information = (char*) malloc( size* sizeof(char));

    char filename[32];
    strcpy(filename, createMessFilename(wsp_id, room_id));
    Message *root = readMessData(filename);
    User *u_root = readUserData("db/users.txt");
    Message *p = root;

    int count_mess = 0;
    char temp[3];
    while (p != NULL)
    {
        if (strstr(p->content, content) != NULL)
        {
            if (count_mess == 10)
            {
                break;
            }
            count_mess++ ;
            // line 0: ID of mess
            sprintf(temp, "%d\n", p->ID);
            strcat(information, temp);

            // line 1: time of mess
            char timestr[64];
            strcpy(timestr, convertTimeTtoString(p->datetime, 1));
            strcat(information, timestr);
            strcat(information, "\n");
            if (p->send_id != user->ID)
            {
                // line 2: send name of mess 
                User *tmp = searchUserByID(u_root, p->send_id);
                strcat(information, tmp->name);
                strcat(information, "\n");
            }
            else
            {
                strcat(information, "0\n");
            }
            if (p->parent_id != 0)
            {
                // line 3: reply
                Message *tmp = findMessByID(root, p->parent_id);
                strcat(information, tmp->content);
                strcat(information, "\n");
            }
            else
            {
                strcat(information, "0\n");
            }
            //line 4: content
            strcat(information, p->content);
            strcat(information, "\n");
        }
        p = p->next;
    }

    if (count_mess == 0)
    {
        strcpy(information, MESS_FIND_ERROR);
    }
    // printf("Information message:\n  %s\n", information);
    return information;
}
char * processResponseForChat(User *user, Message *new, int size)
{
    char * information = (char*) malloc(size * sizeof(char));
    printf("1\n");

    char temp[3];
    sprintf(temp, "%d\n", new->ID);
    strcat(information, temp);

    strcat(information, user->name);
    strcat(information, "\n");
    
    strcat(information, convertTimeTtoString(new->datetime, 1));
    strcat(information, "\n");

    strcat(information, new->content);
    strcat(information, "\n");

    return information;
}
char *processResponseForReply(User *user, Message *new, Message *parent,int size)
{
    //static char information[4096];
    char * information = (char*) malloc(size * sizeof(char));

    char temp[3];
    sprintf(temp, "%d\n", new->ID);
    strcat(information, temp);

    strcat(information, user->name);
    strcat(information, "\n");

    strcat(information, convertTimeTtoString(new->datetime, 1));
    strcat(information, "\n");

    strcat(information, parent->content);
    strcat(information, "\n");

    strcat(information, new->content);
    strcat(information, "\n");

    return information;
}