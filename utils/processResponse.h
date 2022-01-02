#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdio.h>

#include "../models/user.h"
#include "../models/utils.h"
#include "../models/workspace.h"
#include "../models/message.h"
#include "../models/keycode.h"


char * processResponseForViewWSP(User * user);
char * processResponseForJoinWSP(User *user, int wsp_id);
char * processResponseForJoinRoom(User *user, int wsp_id, int room_id);
#endif