#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdio.h>

#include "../models/user.h"
#include "../models/utils.h"
#include "../models/workspace.h"
#include "../models/message.h"
#include "../models/keycode.h"


char * processResponseForViewWSP(User * user, int size);
char * processResponseForJoinWSP(User *user, int wsp_id , int size);
char * processResponseForJoinRoom(User *user, int wsp_id, int room_id, int size);
char * processResponseForChat(User *user, Message *new, int size);
char * processResponseForReply(User *user, Message *new, Message *parent, int size);

char *processResponseForFindDate(User *user, int wsp_id, int room_id, char time[], int size);
char *processResponseForFindContent(User *user, int wsp_id, int room_id, char content[], int size);
#endif