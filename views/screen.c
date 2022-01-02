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
  printf(" 3. Enter %s <workspace_id> to join a workspace.\n", KEY_JOIN);
  printf(" 4. Enter %s to logout the app.\n", KEY_LOGOUT);
  green();
  printf("\n#### -------- THANK YOU --------- ##\n");
  reset();
}
void ScreenViewListWSP(char message[])
{
  int num_line = 0;
  char newLine[5][128];
  splitStringByLine(message, newLine, &num_line);
  if (newLine[0] == 0)
  {
    cyan();
    printf("You don't have any workspaces.\nUse %s <workspace_name> to create your workspace.\n", KEY_NEW);
    reset();
  }
  else
  {
    green();
    printf(" --- YOUR WORKSPACES --- \n");
    reset();
    for (int i = 1; i < num_line; i++)
    {
      if (strlen(newLine[i]) <= 1)
        break;
      int num_word = 0;
      char newString[num_line][16];
      splitString(newLine[i], newString, &num_word);
      printf(" (ID %d) %s ", atoi(newString[0]), newString[1]);
      if (atoi(newString[2]) == 1)
      {
        green();
        printf(" (admin) ");
        reset();
      }
      printf("\n");
    }
  }
}

// after join a chatrooom
void ScreenInWSP(char message[])
{
  //system("clear");

  int num_line = 0;
  char newLine[20][128];
  splitStringByLine(message, newLine, &num_line);
  // for(int i =0; i< num_line; i++)
  // {
  //   printf("%s\n", newLine[i]);
  // }

  green();
  printf(" ---- WELCOME TO %s ---- \n", newLine[0]);
  reset();
  if (atoi(newLine[1]) == 0)
  {
    cyan();
    printf("This workspace don't have any rooms.\n");
    reset();
  }
  else
  {
    printf("WSP rooms: \n");
    for (int i = 0; i < atoi(newLine[1]); i++)
    {
      int id = atoi(strtok(newLine[i + 2], " "));
      char *name = strtok(NULL, "");
      printf(" (ID %d) %s \n", id, name);
    }
  }

  printf("WSP users: \n");
  int num_word = 0;
  char newString[num_line][16];
  splitString(newLine[3 + atoi(newLine[1])], newString, &num_word);
  for (int i = 0; i < num_word - 1; i = i + 2)
  {
    if (strlen(newString[i]) == 0)
      break;
    printf(" (ID %d) %s \n", atoi(newString[i]), newString[i + 1]);
  }

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

void ScreenChat(char message[])
{

  //system("clear");
  green();
  printf("\n---- CHAT ROOM -----\n");
  reset();

  int num_line = 0;
  char newLine[51][128];
  splitStringByLine(message, newLine, &num_line);
  for (int i = 0; i < num_line - 5; i = i + 5)
  {
    printf("%s\n", newLine[i]);

    blue();
    printf("ID %d", atoi(newLine[i]));
    printf("(%s) ", newLine[i + 1]);
    reset();
    if (strcmp(newLine[i + 2], "0") != 0)
    {
      green();
      printf("From %s: ", newLine[i+2]);
      reset();
    }

    if (strcmp(newLine[i + 3], "0") != 0)
    {
      green();
      printf("(Reply %s)", newLine[i+3]);
      reset();
    }
    printf("%s\n", newLine[i+4]);

  }
  printf("(You can enter %s for some instruction.)\n", KEY_HELP);
}

void ScreenChatSearch(Message *root, int user_id, int wsp_id, int room_id, int ids[])
{

  User *u_root = readUserData("db/users.txt");

  //system("clear");
  green();
  printf("\n---- CHAT ROOM -----\n");
  yellow();
  printf("\n\t---- Search result -----\n");
  reset();
  int i = 0;
  Message *p = root;
  while (p != NULL)
  {

    if (p->ID == ids[i])
    {

      i++;
      char timestr[64];
      strcpy(timestr, convertTimeTtoString(p->datetime, 1));

      blue();
      printf("ID %d", p->ID);
      printf("(%s) ", timestr);
      if (p->send_id != user_id)
      {
        green();
        User *tmp = searchUserByID(u_root, p->send_id);
        if (p->parent_id == 0)
          printf("%s: ", tmp->name);
        else
        {
          purple();
          printf("%s reply mess ID %d: ", tmp->name, p->parent_id);
          reset();
        }
      }
      reset();
      printf("%s\n", p->content);
    }

    p = p->next;
  }
  printf("(You can enter %s for some instruction.)\n", KEY_HELP);
}

void DisplayMessage(char message[], char name[])
{
  blue();
  printf("(%s)", getCurrentTime(1));
  green();
  printf("%s: ", name);
  reset();
  printf("%s", message);
}
void DisplayReplyMessage(char message[], char name[], int reply_id)
{
  blue();
  printf("(%s)", getCurrentTime(1));
  green();
  printf("%s reply mess ID %d: ", name, reply_id);
  reset();
  printf("%s", message);
}

void ScreenRoomHelp()
{
  yellow();
  printf("\n (Here is some instructions for you)\n\n");
  reset();
  printf(" 1. Enter %s <message_id> to reply any message.\n", KEY_REPLY);
  printf(" 2. Enter %s <date> to find messages from selected day. Format: #FIND 30/1/2021 \n", KEY_FIND);
  // printf(" 3. Enter %s %s <date from> to find all message from selected day.\n", KEY_FIND, KEY_FROM);
  //printf(" 3. Enter %s %s <date from> %s <date to> to find all message from selected time period.\n", KEY_FIND, KEY_FROM, KEY_TO);
  printf(" 4. Enter %s to lelf the room.\n", KEY_OUTROOM);
}
