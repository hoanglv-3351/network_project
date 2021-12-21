#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include"user.h"

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

char** readCode(int conn_sock) {
    char code[MAX_LENGTH];
    // memset(code, 0, strlen(code));
    //receives username from client
    int codeLen = recv(conn_sock, code, MAX_LENGTH, 0);
    send(conn_sock, "Success code\n", sizeof("Success code\n"), 0);
    code[codeLen-1] = '\0';
    printf("code: %s\n", code);

    char** tokens = NULL;

    tokens = str_split(code, ' ');

    if(tokens == NULL) {
        printf("Read code failure !!!\n");
    }
    else {
        int i=0;

        // print token
        while(tokens[i] != NULL) {
            printf("-- %s --", tokens[i]);
            i++;
        }
        printf("\n");
        // end print

        if(strcmp(tokens[0], "#SIGNUP") == 0) {
            if(tokens[2] == NULL || tokens[3] != NULL) {
                tokens == NULL;
            }
            else {
                // call sign up func
            }
        }
        else if(strcmp(tokens[0], "#LOGIN") == 0) {
            if(tokens[2] == NULL || tokens[3] != NULL) {
                tokens == NULL;
            }
            else {
                // call login func
                // return (User *)logIn(conn_sock, tokens[1], tokens[2]);
            }
        }

        else if(strcmp(tokens[0], "#LOGOUT") == 0) {
            if(tokens[1] != NULL) {
                tokens == NULL;
            }
            else {
                // call logout func
            }
        }

        else if(strcmp(tokens[0], "#NEW") == 0) {
            if(tokens[1] == NULL || tokens[2] != NULL) {
                tokens == NULL;
            }
            else {
                // call new func
            }
        }

        else if(strcmp(tokens[0], "#JOIN") == 0) {
            if(tokens[1] == NULL || tokens[2] != NULL) {
                tokens == NULL;
            }
            else {
                // call join func
            }
        }

        else if(strcmp(tokens[0], "#ADD") == 0) {
            if(tokens[2] == NULL || tokens[3] != NULL) {
                tokens == NULL;
            }
            else {
                // call add func
            }
        }

        else if(strcmp(tokens[0], "#KICK") == 0) {
            if(tokens[1] == NULL || tokens[2] != NULL) {
                tokens == NULL;
            }
            else {
                // call sign up func
            }            
        }

        else if(strcmp(tokens[0], "#SEND") == 0) {
            if(tokens[1] == NULL || tokens[2] != NULL) {
                tokens == NULL;
            }
            else {
                // call send func
            }            
        }

        else if(strcmp(tokens[0], "#REPLY") == 0) {
            if(tokens[2] == NULL || tokens[3] != NULL) {
                tokens == NULL;
            }
            else {
                // call reply func
            }            
        }

        else if(strcmp(tokens[0], "#FIND") == 0) {
            if(tokens[1] == NULL || tokens[2] != NULL) {
                tokens == NULL;
            }
            else {
                // call find func
            }            
        }

        else if(strcmp(tokens[0], "#OUT") == 0) {
            if(tokens[1] != NULL) {
                tokens == NULL;
            }
            else {
                // call out func
            }            
        }

        else if(strcmp(tokens[0], "#EXIT") == 0) {
            if(tokens[1] != NULL) {
                tokens == NULL;
            }
            else {
                // call exit func
            }            
        }

        tokens == NULL;
    }
    return tokens;
}


// int main() {
//     char **token = readCode(0);
// }