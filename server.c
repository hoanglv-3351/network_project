#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#include "views/screen.h"

#include "models/user.h"
#include "models/utils.h"
#include "models/workspace.h"
#include "models/message.h"
#include "models/signal.h"
#include "models/keycode.h"


#include "utils/processResponse.h"

#define MAX_CLIENTS 100
#define BUFFER_SZ 4096

static _Atomic unsigned int cli_count = 0;
//static int uid = 10;

/* Client structure */
typedef struct
{
	struct sockaddr_in address;
	int sockfd;
	User *info;
	int workspace_id;
	int room_id;
} client_t;

client_t *clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_overwrite_stdout()
{
	printf("\r%s", "> ");
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

/* Add clients to queue */
void queue_add(client_t *cli)
{
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (!clients[i])
		{
			clients[i] = cli;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Remove clients to queue */
void queue_remove(int id)
{
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (clients[i]->info->ID == id)
			{
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Send message to all clients except sender */
void send_message_chat(char *s, client_t *cli)
{
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (clients[i]->workspace_id == cli->workspace_id && clients[i]->room_id == cli->room_id)
			{
				if (clients[i]->info->ID != cli->info->ID)
				{
					if (write(clients[i]->sockfd, s, strlen(s)) < 0)
					{
						perror("ERROR: write to descriptor failed");
						break;
					}
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void send_message(char *s, client_t *cli)
{
	pthread_mutex_lock(&clients_mutex);

	if (write(cli->sockfd, s, strlen(s)) < 0)
	{
		perror("ERROR: write to descriptor failed");
	}
	pthread_mutex_unlock(&clients_mutex);
}
void processLOGIN(client_t *cli, char buff_out[], int *flag)
{
	int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
	if (receive <= 0)
	{
		printf("Error input.\n");
		*flag = -1;
		return;
	}

	int num_word = 0;
	char newString[30][16];

	splitString(buff_out, newString, &num_word);
	if (strcmp(newString[0], KEY_LOGIN) != 0)
	{
		send_message("Error format input. Must begin by #LOGIN\n", cli);
		return;
	}
	if (num_word != 3)
	{
		send_message("Error format input. #LOGIN <username> <password>\n", cli);
		return;
	}


	printf("Username: %s - Pasword :  %s\n", newString[1], newString[2]);

	User *root = readUserData("db/users.txt");
	char *response = verifyAccount(root, newString[1], newString[2], flag);
	send_message(response, cli);

	if (*flag != 1)
	{
		return;
	}


	cli->info = searchUserByUsername(root, newString[1]);
	sprintf(buff_out, "%s has joined\n", cli->info->name);
	printf("%s", buff_out);

	char information[BUFFER_SZ];
	sprintf(information, "%d %s %s ", cli->info->ID, cli->info->name,cli->info->password);
	send_message(information, cli);

	//freeUserData(root);
}

void processWorkspace(client_t *cli, char buff_out[], int *flag)
{
	if (cli->workspace_id != -1)
	{
		send_message(MESS_IN_WSP, cli);
		return;
	}
	printf("%s -> %s\n", cli->info->name, buff_out);
	const char s[2] = " ";
	strtok(buff_out, s);
	int wsp_id = atoi(strtok(NULL, s));

	WorkSpace *wsp = readOneWSPData("db/workspaces.txt", wsp_id);
	char *response = checkWSPForUser(wsp, cli->info->ID, flag);
	send_message(response, cli);

	if (*flag != 2)
	{
		return;
	}

	cli->workspace_id = wsp_id;
	printf("%s join wsp %d\n", cli->info->name, cli->workspace_id);
	char information[BUFFER_SZ];
	strcpy(information, processResponseForJoinWSP(cli->info, cli->workspace_id));
	send_message(information, cli);
}

void processChatroom(client_t *cli, char buff_out[], int *flag)
{
	if (cli->room_id != -1)
	{
		send_message(MESS_IN_ROOM, cli);
		return;
	}
	printf("%s -> %s\n", cli->info->name, buff_out);
	const char s[2] = " ";
	strtok(buff_out, s);
	int id = atoi(strtok(NULL, s));

	if (id == cli->info->ID)
	{
		send_message(MESS_ERROR_SELFCHAT, cli);
		return;
	}

	WorkSpace *wsp = readOneWSPData("db/workspaces.txt", cli->workspace_id);
	char *response = checkAvailableID(wsp, id, flag);
	send_message(response, cli);

	if (*flag != 3)
	{
		return;
	}
	if (id % 2 == 1) // connect only a user
	{
		cli->room_id = createFakeRoom(cli->info->ID, id);
	}
	else //connect to a room contains many users
		cli->room_id = id;
	printf("%s join room %d\n", cli->info->name, cli->room_id);
	char information[BUFFER_SZ];
	strcpy(information, processResponseForJoinRoom(cli->info, cli->workspace_id, cli->room_id));
	send_message(information, cli);
	
}

void processMessage(client_t *cli, char buff_out[], int parent_id)
{
	char filename[32];
	strcpy(filename, createMessFilename(cli->workspace_id, cli->room_id));
	printf("filename : %s\n", filename);
	Message *root = readMessData(filename);

	printf("Read Mess Data done.\n");
	printf("Mess time: %s\n", getCurrentTime(2));
	if (root == NULL)
	{
		printf("Read mess null.\n");
		root = createNewMess(parent_id, getCurrentTime(2), cli->info->ID, buff_out);
	}
	else
		root = insertMess(root, parent_id, getCurrentTime(2), cli->info->ID, buff_out);
	writeMessData(root, cli->workspace_id, cli->room_id);
	printf("Write Mess data done.\n");
	//freeMessData(root);
	//printf("Free Mess data done.\n");
}

void processDateFrom(client_t * cli,char date[])
{
	char filename[32];
	strcpy(filename, createMessFilename(cli->workspace_id, cli->room_id));
	Message *root = readMessData(filename);
	Message *p = root;
	time_t time = convertStringToTimeT(date);

	char tmp[BUFFER_SZ];
	char response[BUFFER_SZ];
	while (p!= NULL)
	{
		if (difftime(p->datetime, time) >= 0)
		{
			sprintf(response, " %d", p->ID);
			strcat(tmp, response);
			memset(response, 0, sizeof(response));
		}
		p = p->next;
	}
	if (strlen(tmp) == 0)
	{
		send_message(MESS_SEARCH_ERROR, cli);
		return;
	}
	else
	{
		sprintf(response, "%s", KEY_FIND);
		strcat(response, tmp);
		send_message(response, cli);
		printf("Responf for find mess: %s\n", response);
		return;
	}
}
/* Handle all communication with the client */
void *handle_client(void *arg)
{
	char buff_out[BUFFER_SZ];
	/* 
	-1: error/logout
	0 : begin
	1: login success
	2: in workspce 
	3.: in chatrooom
	*/
	int flag = 0;

	cli_count++;
	client_t *cli = (client_t *)arg;

	while (1)
	{
		printf("Flag 1 = %d\n", flag);
		if (flag == -1 || flag == 1)
		{
			break;
		}
		processLOGIN(cli, buff_out, &flag);
		bzero(buff_out, BUFFER_SZ);
	}

	while (1)
	{
		printf("Flag 2 = %d\n", flag);
		if (flag == -1)
		{
			break; // error -> exit program
		}

		int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0)
		{
			if (strlen(buff_out) > 0)
			{
				printf("%s -> %s\n", cli->info->name, buff_out);
				const char s[2] = " ";
				char tmp[BUFFER_SZ];
				strcpy(tmp, buff_out);
				char *token = strtok(tmp, s);
				if (strcmp(token, KEY_LOGIN) == 0)
				{
					send_message("You have successfully logged in. Not enough login again.\n", cli);
				}
				else if (strcmp(token, KEY_VIEW) == 0)
				{
					send_message(MESS_VIEW_PROFILE, cli);
				}
				else if (strcmp(token, KEY_WSP) == 0)
				{
					send_message(MESS_VIEW_WSP, cli);
					char information[256];
					strcpy(information, processResponseForViewWSP(cli->info));
					send_message(information, cli);
					
				}
				else if (strcmp(token, KEY_JOIN) == 0 && cli->workspace_id == -1)
				{
					processWorkspace(cli, buff_out, &flag);	
				}
				else if (strcmp(token, KEY_JOIN) == 0)
				{
					send_message(MESS_IN_WSP, cli);
				}
				else if (strcmp(token, KEY_CONNECT) == 0 && cli->workspace_id == -1)
				{
					send_message(MESS_JOIN_WSP_WARN, cli);
				}
				else if (strcmp(token, KEY_CONNECT) == 0 && cli->room_id == -1)
				{
					processChatroom(cli, buff_out, &flag);
				}
				else if (strcmp(token, KEY_CONNECT) == 0)
				{
					send_message(MESS_IN_ROOM, cli);
				}
				else if (strcmp(token, KEY_OUTROOM) == 0 && cli->room_id != -1)
				{
					send_message(MESS_OUT_ROOM_SUCCESS, cli);
					char information[512];
					strcpy(information, processResponseForJoinWSP(cli->info, cli->workspace_id));
					send_message(information, cli);
					cli->room_id = -1;
				}
				else if (strcmp(token, KEY_OUTROOM) == 0)
				{
					send_message("You are not in any chatroom.", cli);
				}
				else if (strcmp(token, KEY_OUT) == 0 && cli->workspace_id != -1)
				{
					send_message(MESS_OUT_WSP_SUCCESS, cli);
					cli->workspace_id = -1;
					cli->room_id = -1;
				}
				else if (strcmp(token, KEY_OUT) == 0)
				{
					send_message("You are not in any workspace.", cli);
				}
				// else if (strcmp(token, KEY_REPLY) == 0 && cli->workspace_id != -1 && cli->room_id != -1)
				// {
				// 	printf("Mess reply %s -> %s\n", cli->info->name, buff_out);
				// 	str_trim_lf(buff_out, strlen(buff_out));

				// 	strtok(buff_out, " ");
				// 	int id = atoi(strtok(NULL, " "));
				// 	char *content = strtok(NULL, "");
				// 	processMessage(cli, content, id);

				// 	char tmp[BUFFER_SZ];
				// 	sprintf(tmp, "%s %d %d ", KEY_REPLY, cli->info->ID, id);
				// 	strcat(tmp, buff_out);
				// 	send_message_chat(tmp, cli);
				// }
				// else if (strcmp(token, KEY_FIND) == 0 && cli->workspace_id != -1 && cli->room_id != -1)
				// {
				// 	// int num_word = 0;
				// 	// char newString[5][16];

				// 	// splitString(buff_out, newString, &num_word);
				// 	printf("1\n");
				// 	token = strtok(NULL,s);
				// 	processDateFrom(cli, token);
				// 	// if (num_word == 2 )
				// 	// {
				// 	// 	processDate(newString[1],cli);
				// 	// }
				// 	// else if (num_word == 3 && strcmp(newString[1], KEY_FROM) == 0)
				// 	// {
				// 	// 	processDateFrom(newString[2], cli);
				// 	// }
				// 	// else
				// 	// 	send_message("Wrong format to search. Read instructions again.", cli);
				// }
				// else if (strcmp(token, KEY_HELP) == 0 && cli->workspace_id != -1 && cli->room_id != -1)
				// {
				// 	send_message(buff_out, cli);
				// }
				

				// else if (cli->workspace_id != -1 && cli->room_id != -1)
				// {
				// 	printf("Mess %s -> %s\n", cli->info->name, buff_out);
				// 	str_trim_lf(buff_out, strlen(buff_out));

				// 	if (flag == -1)
				// 	{
				// 		break;
				// 	}
				// 	processMessage(cli, buff_out, 0);

				// 	char tmp[BUFFER_SZ];
				// 	sprintf(tmp, "%d ", cli->info->ID);
				// 	strcat(tmp, buff_out);
				// 	send_message_chat(tmp, cli);
				// }
				else
				{
					send_message(MESS_ERROR, cli);
					//str_trim_lf(buff_out, strlen(buff_out));
				}
			}
		}
		else if (receive == 0 || strcmp(buff_out, KEY_LOGOUT) == 0)
		{
			sprintf(buff_out, "%s logout succesfully\n", cli->info->name);
			printf("%s", buff_out);
			send_message(buff_out, cli);
			flag = -1;
		}
		else
		{
			printf("ERROR: -1\n");
			flag = -1;
		}

		bzero(buff_out, BUFFER_SZ);
	}

	/* Delete client from queue and yield thread */
	close(cli->sockfd);
	queue_remove(cli->info->ID);
	//free(cli);
	cli = NULL;
	cli_count--;
	pthread_detach(pthread_self());

	return NULL;
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
	int option = 1;
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t tid;

	/* Socket settings */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port);

	/* Ignore pipe signals */
	signal(SIGPIPE, SIG_IGN);

	if (setsockopt(listenfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&option, sizeof(option)) < 0)
	{
		perror("ERROR: setsockopt failed");
		return EXIT_FAILURE;
	}

	/* Bind */
	if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR: Socket binding failed");
		return EXIT_FAILURE;
	}

	/* Listen */
	if (listen(listenfd, 10) < 0)
	{
		perror("ERROR: Socket listening failed");
		return EXIT_FAILURE;
	}

	printf("=== WELCOME TO THE CHAT APPLICATION ===\n");

	while (1)
	{
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &clilen);

		/* Check if max clients is reached */
		if ((cli_count + 1) == MAX_CLIENTS)
		{
			printf("Max clients reached. Rejected: ");
			close(connfd);
			continue;
		}
		printf("Accept connection from ");
		print_ip_addr(cli_addr);
		printf(":%d\n", cli_addr.sin_port);

		/* Client settings */
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->address = cli_addr;
		cli->sockfd = connfd;
		cli->workspace_id = -1;
		cli->room_id = -1;

		/* Add client to the queue and fork thread */
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void *)cli);

		/* Reduce CPU usage */
		sleep(1);
	}

	return EXIT_SUCCESS;
}