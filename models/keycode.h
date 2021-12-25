#ifndef __KEYCODE_H__
#define __KEYCODE_H__

typedef enum {
  KEY_SIGNUP, // #SIGNUP <username> <password>
  KEY_LOGIN, // #LOGIN <username> <password> 
  KEY_LOGOUT, // #LOGOUT


  KEY_VIEW, // view user profile #VIEW (optinal <user_id>)

  KEY_WSP, // view all workspace user blongs to
  KEY_NEW, // admin create a wsp: #NEW <workspace_name>
  KEY_JOIN, // join a wsp: #JOIN <workspace_id>
  KEY_ADD, // admin add a user to a wsp: #ADD <workspace_id> <user_id> 
  KEY_KICK, // admin kick user to a wsp:  #KICK <workspace_id> <user_id>



  KEY_SEND, // #SEND <content>
  KEY_REPLY, // #REPLY <message_id> <content>
  KEY_FIND, // #FIND <date>
  KEY_OUT, // #OUT
  KEY_EXIT, // #EXIT
} KeyCode;
 
#define KEY_SIGNUP "#SIGNUP"
#define KEY_LOGIN "#LOGIN"
#define KEY_LOGOUT "#LOGOUT"

#define KEY_VIEW "#VIEW"

#define KEY_WSP "#WSP_LIST"
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