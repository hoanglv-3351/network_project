#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SZ 4096

#include "views/screen.h"
#include "models/utils.h"
#include "models/signal.h"
#include "models/user.h"
#include "models/workspace.h"
#include "models/keycode.h"

// Global variables
volatile sig_atomic_t flag = 0;
int sockfd = 0;

typedef struct
{
	User *info;
	int workspace_id;
	int room_id;
} client_t;

client_t *cli;

int wsp_id = 0;
int room_id = 0;

void str_overwrite_stdout()
{
	green();
	printf("\n%s", ">");
	reset();
	fflush(stdout);
}

void str_trim_lf(char *arr, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{ // trim \n
		if (arr[i] == '\n')
		{
			arr[i] = '\0';
			break;
		}
	}
}

void catch_ctrl_c_and_exit(int sig)
{
	flag = 1;
}

void send_msg_handler()
{

	char buffer[BUFFER_SZ];
	while (1)
	{
		str_overwrite_stdout();
		fgets(buffer, BUFFER_SZ, stdin);
		str_trim_lf(buffer, BUFFER_SZ);

		if (strcmp(buffer, KEY_LOGOUT) == 0)
		{
			break;
		}
		else
		{
			send(sockfd, buffer, strlen(buffer), 0);

			if (strstr(buffer, KEY_JOIN))
			{
				const char s[2] = " ";
				char *token = strtok(buffer, s);
				token = strtok(NULL, s);
				wsp_id = atoi(token);
			}
			if (strstr(buffer, KEY_CONNECT))
			{
				const char s[2] = " ";
				char *token = strtok(buffer, s);
				token = strtok(NULL, s);
				room_id = atoi(token);
			}
		}
		bzero(buffer, BUFFER_SZ);
	}
	catch_ctrl_c_and_exit(2);
}
void process_message(char message[])
{
	if (strcmp(message, MESS_LOGIN_SUCCESS) == 0)
	{

		memset(message, 0, BUFFER_SZ);
		recv(sockfd, message, BUFFER_SZ, 0);
		int num_word = 0;
		char newString[30][16];
		splitString(message, newString, &num_word);
		cli->info->ID = atoi(newString[0]);
		strcpy(cli->info->name, newString[1]);
		strcpy(cli->info->password, newString[2]);

		printf("Welcome %s\n", cli->info->name);
		ScreenLoginSuccess();
	}
	else if (strcmp(message, MESS_VIEW_PROFILE) == 0)
	{
		printf("Your name is : %s\n", cli->info->name);
		printf("Your id is : %d\n", cli->info->ID);
		printf("Your password is %s\n", cli->info->password);
	}
	else if (strcmp(message, MESS_VIEW_WSP) == 0)
	{
		memset(message, 0, BUFFER_SZ);
		recv(sockfd, message, BUFFER_SZ, 0);
		ScreenViewListWSP(message);
	}
	else if (strcmp(message, MESS_JOIN_WSP_SUCCESS) == 0)
	{
		cli->workspace_id = wsp_id;
		memset(message, 0, BUFFER_SZ);
		recv(sockfd, message, BUFFER_SZ, 0);
		ScreenInWSP(message);
	}
	else if (strcmp(message, MESS_JOIN_ROOM_SUCCESS) == 0)
	{
		printf("%s", message);
		if (room_id % 2 == 1) // connect only a user
		{
			cli->room_id = createFakeRoom(cli->info->ID, room_id);
		}
		else //connect to a room contains many users
			cli->room_id = room_id;
		memset(message, 0, BUFFER_SZ);
		recv(sockfd, message, BUFFER_SZ, 0);
		ScreenChat(message);

	}
	else if (strcmp(message, MESS_OUT_ROOM_SUCCESS) == 0)
	{
		printf("%s", message);
		memset(message, 0, BUFFER_SZ);
		recv(sockfd, message, BUFFER_SZ, 0);
		ScreenInWSP(message);
		cli->room_id = -1;
	}
	else if (strcmp(message, MESS_OUT_WSP_SUCCESS) == 0)
	{

		printf("%s", message);
		ScreenLoginSuccess();
		cli->workspace_id = -1;
		cli->room_id = -1;
	}
	else if (strcmp(message, KEY_HELP) == 0)
	{
		ScreenRoomHelp();
	}
	else if (strstr(message, KEY_REPLY) && cli->room_id != -1 && cli->workspace_id != -1)
	{
		char *token = strtok(message, " ");
		int send_id = atoi(strtok(NULL, " "));
		int reply_id = atoi(strtok(NULL, " "));
		token = strtok(NULL, "");
		User *u_root = readUserData("db/users.txt");
		User *user = searchUserByID(u_root, send_id);

		DisplayReplyMessage(token, user->name, reply_id);
	}
	else if (strstr(message, KEY_FIND) && cli->room_id != -1 && cli->workspace_id != -1)
	{
		int num_word = 0;
		char newString[30][16];

		splitString(message, newString, &num_word);
		int ids[64];
		for (int i = 1; i < num_word; i++)
		{
			ids[i - 1] = atoi(newString[i]);
		}

		char filename[32];
		strcpy(filename, createMessFilename(cli->workspace_id, cli->room_id));
		Message *root = readMessData(filename);
		ScreenChatSearch(root, cli->info->ID, cli->workspace_id, cli->room_id, ids);
	}
	else if (strstr(message, KEY_HELP) == 0 && cli->room_id != -1 && cli->workspace_id != -1)
	{
		ScreenRoomHelp();
	}

	else if (cli->room_id != -1 && cli->workspace_id != -1)
	{
		char *token = strtok(message, " ");
		int send_id = atoi(token);
		token = strtok(NULL, "");
		User *u_root = readUserData("db/users.txt");
		User *user = searchUserByID(u_root, send_id);

		DisplayMessage(token, user->name);
	}

	else
	{
		printf("%s", message);
	}
}
void recv_msg_handler()
{
	char message[BUFFER_SZ] = {};
	while (1)
	{
		int receive = recv(sockfd, message, BUFFER_SZ, 0);
		if (receive > 0)
		{
			process_message(message);
			str_overwrite_stdout();
		}
		else if (receive == 0)
		{
			break;
		}
		else
		{
			// -1
		}
		memset(message, 0, sizeof(message));
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Usage: %s <ip address> <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char ip[16];
	strcpy(ip, argv[1]);
	int port = atoi(argv[2]);
	//char buffer[BUFFER_SZ];

	signal(SIGINT, catch_ctrl_c_and_exit);

	ScreenLogin();
	//fgets(buffer, BUFFER_SZ, stdin);
	//str_trim_lf(buffer, strlen(buffer));

	struct sockaddr_in server_addr;

	/* Socket settings */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	// Connect to Server
	int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err == -1)
	{
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}

	/* Client settings */
	cli = (client_t *)malloc(sizeof(client_t));
	cli->info = (User *)malloc(sizeof(User));
	cli->workspace_id = -1;
	cli->room_id = -1;

	// Send message
	//send(sockfd, buffer, sizeof(buffer), 0);

	pthread_t send_msg_thread;
	if (pthread_create(&send_msg_thread, NULL, (void *)send_msg_handler, NULL) != 0)
	{
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	pthread_t recv_msg_thread;
	if (pthread_create(&recv_msg_thread, NULL, (void *)recv_msg_handler, NULL) != 0)
	{
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	while (1)
	{
		if (flag)
		{
			printf("\nLogout succefully\n");
			break;
		}
	}

	close(sockfd);

	return EXIT_SUCCESS;
}