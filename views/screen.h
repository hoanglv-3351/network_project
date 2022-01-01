#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdio.h>

#include "../models/user.h"
#include "../models/utils.h"
#include "../models/workspace.h"
#include "../models/message.h"
#include "../models/keycode.h"


void red () ;

void green ();
void yellow ();
void blue ();
void purple ();
void cyan ();
void white ();

void reset ();

void ScreenLogin();
void ScreenLoginSuccess();
void ScreenInWSP();

void ChatScreen(Message *root, int user_id, int wsp_id, int room_id);
void DisplayMessage(char message[], char name[]);
void ScreenRoomHelp();

#endif