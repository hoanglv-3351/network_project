#include "screen.h"

#include <string.h>
#include <stdlib.h>

#include "../models/user.h"
#include "../models/workspace.h"
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

//After login
void ScreenLoginSuccess()
{

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
// after join a workspace
void ScreenJoinRoom()
{
}

// after join a chatrooom
void ScreenInWSP(int wsp_id)
{

  WorkSpace *wsp = readOneWSPData("db/workspaces.txt", wsp_id);
  User * root = readUserData("db/users.txt");

  green();
  printf(" ---- WELCOME TO %s ---- \n", wsp->name);
  reset();
  printf(" WSP rooms: \n");
  printf(" WSP users: \n");
  for (int i = 0; i < wsp->num_of_users; i++)
  {
    User * p = searchUserByID(root, wsp->user_id[i]);
    printf(" (ID %d) %s\n",p->ID, p->name);
  }

  yellow();
  yellow();
  printf("\n (Here is some instructions for you)\n");
  reset();
  printf(" 1. Enter %s <user_id/room_id> to join chatroom\n", KEY_CONNECT);
  printf(" 2. Enter %s <username/roomname> to search a user/ a room<\n", KEY_SEARCH);
  printf(" 3. Enter %s to leave out current workspace.\n", KEY_OUT);
  green();
  printf("\n#### -------- THANK YOU --------- ##\n");
  reset();

}