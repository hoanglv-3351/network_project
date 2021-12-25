#include "screen.h"

#include <string.h>
#include <stdlib.h>

void red () {
  printf("\033[1;31m");
}
void green (){
  printf("\033[1;32m");
}
void yellow (){
  printf("\033[1;33m");
}
void blue (){
  printf("\033[1;34m");
}

void purple (){
  printf("\033[1;35m");
}

void cyan (){
  printf("\033[1;36m");
}

void white (){
  printf("\033[1;37m");
}

void reset () {
  printf("\033[0m");
}

//After login 
void ScreenLoginSuccess(){
    

    green();
    printf( "\n ------ WELCOME WORKSPACE APP -----\n");
    yellow();
    printf( "\n (Here is some instructions for you)\n\n");
    reset();
    printf( " 1. Enter #VIEW to view your profile\n");
    printf( " 2. Enter #WPS_LIST to view all of your workspaces\n");
    printf( " 3. Enter #JOIN <workspace_name> to join a workspace.\n");
    printf( " 4. Enter #LOGOUT to logout the app.\n");
    green();
    printf( "\n#### -------- THANK YOU --------- ##\n");
    reset();

}

// after join a workspace 
void ScreenJoinRoom(){


}


// after join a chatrooom
void ScreenInWorkSpace(){

}