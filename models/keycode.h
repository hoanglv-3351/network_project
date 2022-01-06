#ifndef __KEYCODE_H__
#define __KEYCODE_H__

// typedef enum {
//   KEY_SIGNUP, // #SIGNUP <username> <password>
//   KEY_LOGIN, // #LOGIN <username> <password> 
//   KEY_LOGOUT, // #LOGOUT


//   KEY_VIEW, // view user profile #VIEW (optinal <user_id>)

//   KEY_WSP, // view all workspace user blongs to
//   KEY_NEW, // admin create a wsp: #NEW <workspace_name>
//   KEY_JOIN, // join a wsp: #JOIN <workspace_id>
//   KEY_ADD, // admin add a user to a wsp: #ADD <workspace_id> <user_id> 
//   KEY_KICK, // admin kick user to a wsp:  #KICK <workspace_id> <user_id>

//   KEY_CONNECT, // connect to a user/room to join a chat room: #CONNECT <user_id>
//   KEY_SEARCH, // search a user or a room: #SEARCH <name> 


//   KEY_SEND, // #SEND <content>
//   KEY_REPLY, // #REPLY <message_id> <content>
//   KEY_FIND, // #FIND <date> find message by date
//   KEY_OUTROOM, // #OUT out chat room
//   KEY_EXIT, // #EXIT
// } KeyCode;
 
#define KEY_SIGNUP "#SIGNUP"
#define KEY_LOGIN "#LOGIN"
#define KEY_LOGOUT "#LOGOUT"

// in login success screen
#define KEY_VIEW "#VIEW"
#define KEY_WSP "#LIST_WSP"
#define KEY_JOIN "#JOIN"
#define KEY_NOTICE "#NOTICE"
#define KEY_NOTICE_ALL "#NOTICE_ALL"

// in wsp command
#define KEY_CONNECT "#CONNECT"
#define KEY_SEARCH "#SEARCH"
#define KEY_OUT "#OUT"

// in wsp command for admin
#define KEY_NEW "#NEW"
#define KEY_ADD "#ADD"
#define KEY_KICK "#KICK"

// in chat room command
#define KEY_REPLY "#REPLY"
#define KEY_FROM "#FROM"
#define KEY_TO "#TO"
#define KEY_FIND "#FIND"
#define KEY_FIND_CONTENT "#S"
#define KEY_OUTROOM "#OUTROOM"
#define KEY_EXIT "#EXIT"



#define KEY_HELP "#HELP"
#endif