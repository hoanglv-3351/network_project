#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/socket.h>
#include <sys/types.h>

#include <time.h>


void print_ip_addr(struct sockaddr_in addr);

int createFakeRoom(int a, int b);
int returnFakeRoomToID(int room_id, int cli_id);


time_t convertStringToTimeT(char time[]);
char * convertTimeTtoString(time_t converted, int type);
char * getCurrentTime(int type);
#endif