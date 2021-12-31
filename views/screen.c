#include "screen.h"

#include <string.h>
#include <stdlib.h>

#include "../models/user.h"
#include "../models/utils.h"
#include "../models/workspace.h"
#include "../models/message.h"
#include "../models/keycode.h"

void red()
{
  printf("\033[1;31m");
}
void green()
{
  printf("\033[1;32m");
}
void yellow()
{
  printf("\033[1;33m");
}
void blue()
{
  printf("\033[1;34m");
}

void purple()
{
  printf("\033[1;35m");
}

void cyan()
{
  printf("\033[1;36m");
}

void white()
{
  printf("\033[1;37m");
}

void reset()
{
  printf("\033[0m");
}

void ScreenLogin()
{
  system("clear");
  green();
  printf("   === WELCOME ===\n\n");
  reset();
  printf("Please LOGIN. Command %s <username> <password>.\n", KEY_LOGIN);
  printf("Or use %s <username> <password> to signup a account.\n\n", KEY_SIGNUP);
}

//After login
void ScreenLoginSuccess()
{
  //system("clear");
  green();
  printf("\n ------ WELCOME WORKSPACE APP -----\n");
  yellow();
  printf("\n (Here is some instructions for you)\n\n");
  reset();
  printf(" 1. Enter %s to view your profile\n", KEY_VIEW);
  printf(" 2. Enter %s to view all of your workspaces\n", KEY_WSP);
  printf(" 3. Enter %s <workspace_name> to join a workspace.\n", KEY_JOIN);
  printf(" 4. Enter %s to logout the app.\n", KEY_LOGOUT);
  green();
  printf("\n#### -------- THANK YOU --------- ##\n");
  reset();
}
void ScreenProfile(User *id)
{
}

// after join a chatrooom
void ScreenInWSP(int wsp_id)
{
  //system("clear");
  WorkSpace *wsp = readOneWSPData("db/workspaces.txt", wsp_id);
  User *root = readUserData("db/users.txt");

  green();
  printf(" ---- WELCOME TO %s ---- \n", wsp->name);
  reset();
  printf(" WSP rooms: \n");
  printf(" WSP users: \n");
  for (int i = 0; i < wsp->num_of_users; i++)
  {
    User *p = searchUserByID(root, wsp->user_id[i]);
    printf(" (ID %d) %s\n", p->ID, p->name);
  }

  yellow();
  yellow();
  printf("\n (Here is some instructions for you)\n");
  reset();
  printf(" 1. Enter %s <user_id/room_id> to join chatroom\n", KEY_CONNECT);
  printf(" 2. Enter %s <username/roomname> to search a user/ a room<\n", KEY_SEARCH);
  printf(" 3. Enter %s to left the current workspace.\n", KEY_OUT);
  green();
  printf("\n#### -------- THANK YOU --------- ##\n");
  reset();
}

void ChatScreen(Message *root, int user_id, int wsp_id, int room_id)
{

  User *u_root = readUserData("db/users.txt");

  //system("clear");
  green();
  printf("\n---- CHAT ROOM -----\n");
  reset();
  
  Message *p = root;
  while (p != NULL)
  {
    char timestr[64];
    strcpy(timestr, convertTimeTtoString(p->datetime, 1));

    //printf("(%d)", p->ID);
    blue();
    printf("(%s) ", timestr);
    if (p->send_id != user_id)
    {
      green();
      User *tmp = searchUserByID(u_root, p->send_id);
      printf("%s: ", tmp->name);
      reset();
    }
    printf("%s\n", p->content);
    p = p->next;
  }
}

void DisplayMessage(Message *root, char message[], char name[])
{
    blue();
		printf("(%s)", getCurrentTime(1));
    green();
    printf("%s: ",name);
    reset();
		printf("%s", message);
}

void ScreenRoomHelp()
{
  yellow();
  printf("\n (Here is some instructions for you)\n\n");
  reset();
  printf(" 1. Enter %s <message_id> to reply any message.\n", KEY_REPLY);
  printf(" 2. Enter %s <date> to find messages in selected day. Ex: #FIND 30/1/2021 \n", KEY_FIND);
  printf(" 3. Enter %s %s <date from> to find all message from selected day.\n", KEY_FIND, KEY_FROM);
  printf(" 3. Enter %s %s <date from> %s <date to> to find all message from selected time period.\n", KEY_FIND, KEY_FROM, KEY_TO);
  printf(" 4. Enter %s to lelf the room.\n", KEY_OUTROOM);
}

