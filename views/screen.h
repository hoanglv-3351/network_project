#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdio.h>

#include "../models/user.h"
#include "../utils/utils.h"
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
void ScreenViewListWSP(char message[]);
void ScreenInWSP(char message[]);

void ScreenChat(char message[]);

void ScreenChatSearch(Message *root, int user_id, int wsp_id, int room_id, int ids[]);
void DisplayMessage(char message[]);
void DisplayReplyMessage(char message[]);
void ScreenRoomHelp();

#endif