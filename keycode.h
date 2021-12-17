#ifndef __KEYCODE_H__
#define __KEYCODE_H__

typedef enum {
  KEY_SIGNUP, // #SIGNUP <username> <password>
  KEY_LOGIN, // #LOGIN <username> <password> 
  KEY_LOGOUT, // #LOGOUT
  KEY_NEW, // #NEW <workspace_name>
  KEY_JOIN, // #JOIN <workspace_id>
  KEY_ADD, // #ADD <user_id> <workspace_id>
  KEY_KICK, // #KICK <user_id>
  KEY_SEND, // #SEND <content>
  KEY_REPLY, // #REPLY <message_id> <content>
  KEY_FIND, // #FIND <date>
  KEY_OUT, // #OUT
  KEY_EXIT, // #EXIT
} KeyCode;
 
#define KEY_SIGNUP "#SIGNUP"
#define KEY_LOGIN "#LOGIN"
#define KEY_LOGOUT "#LOGOUT"
#define KEY_NEW "#NEW"
#define KEY_JOIN "#JOIN"
#define KEY_ADD "#ADD"
#define KEY_KICK "#KICK"
#define KEY_SEND "#SEND"
#define KEY_REPLY "#REPLY"
#define KEY_FIND "#FIND"
#define KEY_OUT "#OUT"
#define KEY_EXIT "#EXIT"

#endif
