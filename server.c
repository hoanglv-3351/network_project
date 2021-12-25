#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>


#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#include "models/user.h"
#include "models/utils.h"
#include "views/screen.h"
#include "models/signal.h"
#include "models/keycode.h"

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048

static _Atomic unsigned int cli_count = 0;
static int uid = 10;

/* Client structure */
typedef struct
{
	struct sockaddr_in address;
	int sockfd;
	User *info;
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
void queue_remove(int uid)
{
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (clients[i]->info->ID == uid)
			{
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Send message to all clients except sender */
void send_message_chat(char *s, int uid)
{
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (clients[i]->info->ID != uid)
			{
				if (write(clients[i]->sockfd, s, strlen(s)) < 0)
				{
					perror("ERROR: write to descriptor failed");
					break;
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
void processLOGIN(client_t *cli, char buff_out[], int *leave_flag)
{

	int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
	if (receive <= 0)
	{
		printf("Error input.\n");
		*leave_flag = 2;
		return;
	}

	const char s[2] = " ";
	char *token, *username, *password;
	token = strtok(buff_out, s);
	if (strcmp(token, KEY_LOGIN) != 0)
	{
		send_message("Error input. Must begin by #LOGIN\nEnter: ", cli);
		*leave_flag = 2;
		return;
	}

	username = strtok(NULL, s);
	password = strtok(NULL, s);
	printf("Username: %s - Pasword :  %s\n", username, password);

	User *root = readUserFile("db/users.txt");
	int logInStatus = 0; // 0 is success, 1 is wrong pass, 2 is account not exist
	char *response = verifyAccount(root, username, password, &logInStatus);
	send_message(response, cli);

	if (logInStatus != 0)
	{
		return;
	}

	*leave_flag = 1; // 1 is login success
	cli->info = searchUserByUsername(root, username);
	sprintf(buff_out, "%s has joined\n", username);
	printf("%s", buff_out);
}

void processWorkspace(client_t *cli, char buff_out[], int *leave_flag)
{
	printf("%s -> %s\n", cli->info->name, buff_out);
	const char s[2] = " ";
	char * token = strtok(buff_out, s);
	int wsp_id = atoi(strtok(NULL, s));

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
	int leave_flag = 0; // 1 is login success, 2 is error/logout.

	cli_count++;
	client_t *cli = (client_t *)arg;

	// Name

	while (1)
	{
		if (leave_flag == -1 || leave_flag == 1)
		{
			break;
		}
		processLOGIN(cli, buff_out, &leave_flag);
		bzero(buff_out, BUFFER_SZ);
	}

	while (1)
	{
		if (leave_flag == -1)
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
					send_message("You have successfully logged in.\n", cli);
				}
				else if (strcmp(token, KEY_JOIN) == 0)
				{
					processWorkspace(cli, buff_out, &leave_flag);
				}
				else if (strcmp(token, KEY_VIEW) == 0)
				{
					send_message(MESS_VIEW_PROFILE, cli);
				}
				else if (strcmp(token, KEY_WSP) == 0)
				{

				}

				else {

				char name[32];
				char tmp[BUFFER_SZ];
				strcpy(tmp, buff_out);
				strcpy(name, cli->info->name);
				strcpy(name, strcat(name, " send: "));

				strcpy(buff_out, strcat(name, tmp));

				send_message_chat(buff_out, cli->info->ID);

				//str_trim_lf(buff_out, strlen(buff_out));
				
				}
			}
		}
		else if (receive == 0 || strcmp(buff_out, KEY_LOGOUT) == 0)
		{
			sprintf(buff_out, "%s Logout succesfully\n", cli->info->name);
			printf("%s", buff_out);
			send_message(buff_out, cli);
			leave_flag = -1;
		}
		else
		{
			printf("ERROR: -1\n");
			leave_flag = -1;
		}

		bzero(buff_out, BUFFER_SZ);
	}

	/* Delete client from queue and yield thread */
	close(cli->sockfd);
	queue_remove(cli->info->ID);
	free(cli);
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
		print_ip_addr(cli_addr);
		printf(":%d\n", cli_addr.sin_port);

		/* Client settings */
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->address = cli_addr;
		cli->sockfd = connfd;

		/* Add client to the queue and fork thread */
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void *)cli);

		/* Reduce CPU usage */
		sleep(1);
	}

	return EXIT_SUCCESS;
}