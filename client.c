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

#define BUFFER_SZ 2048

#include "views/screen.h"
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

char username[10];
void str_overwrite_stdout()
{
	green();
	printf("%s", ">");
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
			if (strstr(buffer, KEY_LOGIN))
			{
				const char s[2] = " ";
				char *token = strtok(buffer, s);
				token = strtok(NULL, s);
				strcpy(username, token);
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
		User *root = readUserFile("db/users.txt");
		cli->info = searchUserByUsername(root, username);

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
		WorkSpace *root = readWorkspaceData("db/workspaces.txt");
		//printAllWSP(root);

		int count = 0;
		int *list_wps = findWSPForUser(root, cli->info->ID, &count);

		if (count == 0)
		{
			cyan();
			printf("You don't have any workspaces.\nUse %s to create your workspace.\n", KEY_NEW);
			reset();
		}
		else
		{
			printf("YOUR WORKSPACES\n");
			for (int i = 0; i < count; i++)
			{
				{
					WorkSpace *tmp = searchWSPByID(root, list_wps[i]);
					printf(" (ID %d) %s ", tmp->ID, tmp->name);
					if (tmp->host_id == cli->info->ID)
					{
						green();
						printf(" (admin) ");
						reset();
					}
					printf("\n");
				}
			}
		}
	}
	else if (strcmp(message, MESS_JOIN_WSP_SUCCESS) == 0)
	{
		ScreenInWorkSpace();
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
	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);
	//char buffer[BUFFER_SZ];

	signal(SIGINT, catch_ctrl_c_and_exit);

	printf("=== WELCOME TO THE CHATROOM ===\n");
	printf("Please LOGIN. Command #LOGIN <username> <password>.\n");
	printf("Enter: ");
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