
#include <stdio.h> /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "user.h"
#include "functions.c"
#include "readCode.c"

#define BACKLOG 2 /* Number of allowed connections */
#define BUFF_SIZE 1024

// max length of a string
#define MAX 256

int main(int argc, char const *argv[])
{
    //signal();
    if (argc != 2)
    {
        printf("error, too many or too few arguments\n");
        printf("Correct format is /.server YourPort\n");
        return 1;
    }
    updateListUser();
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int listen_sock, conn_sock; /* file descriptors */

    struct sockaddr_in server; /* server's address information */
    struct sockaddr_in client; /* client's address information */
    int sin_size;

    //Step 1: Construct a TCP socket to listen connection request
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    { /* calls socket() */
        perror("\nError: ");
        return 0;
    }
    printf("[+]Server Socket is created.\n");

    //Step 2: Bind address to socket
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));     /* Remember htons() from "Conversions" section? =) */
    server.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */
    if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    { /* calls bind() */
        perror("\nError: ");
        return 0;
    }
    printf("[+]Bind to port %s\n", argv[1]);

    //Step 3: Listen request from client
    if (listen(listen_sock, BACKLOG) == -1)
    { /* calls listen() */
        perror("\nError: ");
        return 0;
    }
    else printf("[+]Listening....\n");
    //PrintList();
    int pid;
    
    
    
    //Step 4: Communicate with client
    int isLoggedin = 0;
    User *user = NULL;
    char msg[MAX_LENGTH];




    while (1)
    {
        //accept request
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1)
            perror("\nError: ");

        printf("You got a connection from %s\n", inet_ntoa(client.sin_addr)); /* prints client's IP */
        
        pid = fork();
        if (pid == -1)
        {
            close(conn_sock);
            printf("ERROR in new process creation\n");
            exit(1);
        }
        else if(pid > 0 )
        {//parent
            close(conn_sock);
            continue;
        }
        else if (pid == 0) {
            //kill() gui tin hieu den progress khac
            //child process
            //do whatever you want
            //handleSignal()
            
            printf("Receiving data ...\n");

            char **tokens = NULL; 
            while(tokens == NULL) {

                tokens = readCode(conn_sock);

                if(strcmp(tokens[0], "#LOGIN") == 0) {
                    if(user == NULL){
                        printf("Username: %s\n", tokens[1]);
                        printf("Password: %s\n", tokens[2]);
                        sleep(1);
                        user = logIn(conn_sock, tokens[1], tokens[2]);
                        printf("-------------------------\n");
                    }
                    else {
                        printf("User has been logged in.\n");
                        strcpy(msg, "OK");
                        send(conn_sock, msg, strlen(msg), 0);
                        tokens = NULL;
                    }
                }

                else if(strcmp(tokens[0], "#SIGNUP") == 0) {
                    signUp(conn_sock, tokens[1], tokens[2]);
                }


                else {
                    tokens = NULL;
                    // strcpy(msg, "Wrong code");
                    // send(conn_sock, msg, strlen(msg), 0);
                }
            }

            // // SIGNUP
            // while(1){
            //     char username[BUFF_SIZE];
            //     char password[BUFF_SIZE];
            //     memset(username, 0, strlen(username));
            //     memset(password, 0, strlen(password));
            //     printf("Receiving data ...\n");
            //     //receives username from client
            //     recv(conn_sock, username, BUFF_SIZE, 0);
            //     send(conn_sock, "Received username\n", sizeof("Received username\n"), 0);
            //     username[strlen(username)-1] = '\0';
            //     printf("Username: %s\n", username);
            //     //receives password from client
            //     recv(conn_sock, password, BUFF_SIZE, 0);
            //     send(conn_sock, "Received password\n", sizeof("Received password\n"), 0);
            //     password[strlen(password)-1] = '\0';
            //     printf("Password: %s\n", password);
            //     printf("data is here!\n");
            //     sleep(1);
            //     signUp(conn_sock, username, password);
            //     printf("-------------------------\n");
            // }
            // // END SIGNUP

            // // LOGIN
            // while(user == NULL){
            //     printf("Receiving data ...\n");
            //     // receives username from client
            //     recv(conn_sock, username, BUFF_SIZE, 0);
            //     send(conn_sock, "Received username\n", sizeof("Received username\n"), 0);
            //     username[strlen(username)-1] = '\0';
            //     printf("Username: %s\n", username);
            //     //receives password from client
            //     recv(conn_sock, password, BUFF_SIZE, 0);
            //     send(conn_sock, "Received password\n", sizeof("Received password\n"), 0);
            //     password[strlen(password)-1] = '\0';
            //     printf("Password: %s\n", password);
            //     printf("data is here!\n");
            //     sleep(1);
            //     user = logIn(conn_sock, username, password);
                
            //     printf("-------------------------\n");
            // }
            // // END LOGIN

            // // LOGOUT
            // user->isLogin = 0;
            // free(user);      
            // if(user == NULL) {
            //     printf("logout successfull ! \n");
            // }      
            // // END LOGOUT


            close(conn_sock);
            break;
        }
    }
    close(listen_sock);
    return 0;
}
