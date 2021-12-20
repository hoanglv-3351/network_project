#include<stdio.h>
#include<string.h>

#include "user.c"


extern int toupper (int __c) __THROW;


int isValid(char *str) {
    int valid = 1;
    int idx = 0;
    for(idx ; idx < strlen(str) ; idx++) {
        char c = (char)toupper(str[idx]);
        if((c <= 90 && c >= 65) || (c >= 48 && c <= 57)) continue;
        else {
            valid = 0;
            break;
        }
    }
    return valid;
}

void signUp(char *name, char *password) {
    updateListUser(); //

    if(!isValid(name) || !isValid(password)) {
        printf("Invalid name or password!\n");
        return;
    }

    if(isExistingUser(name)) {
        printf("This user is already exist!\n");
    }
    else {
        number_of_users++;
        FILE *f;
        if (!(f = fopen("db/users.txt", "w"))) {
            printf("\n File not found!! \n\n");
        }
        else {
            fprintf(f, "%d", number_of_users);
            User *user = (User *)malloc(sizeof(User));
            user->ID = number_of_users;
            strcpy(user->name, name);
            strcpy(user->password, password);    
            insertUser(user);

            User *ptr = headUser;
            while (ptr != NULL) {
                fprintf(f, "\n%d\n%s\n%s", ptr->ID, ptr->name, ptr->password);
                ptr = ptr->next;
            }
            free(ptr);
        }
        fclose(f);
        printf("Account %s is sign up successfully!\n", name);
    }
}

void logIn(char *name, char *password) {
    printf("---Log in---\n");
    printf("Username: %s\n", name);
    printf("Password: %s\n", password);

    do {
        User *user;
        user = headUser;
        while (user != NULL) {
            //check if username is exist or not
            if (isExistingUser(name)){
                // check password
                if (strcmp(password, user->password) != 0) {
                    char *msg = "Password is incorrect.\n";
                    // send(conn_sock, msg, strlen(msg), 0);
                    printf("%s", msg);
                }
                // login success
                else {
                    user->isLogin = 1;
                    char *msg = "Hello! Successful login.\n";
                    //strcat(msg, ptr->m_username);
                    // send(conn_sock, msg, strlen(msg), 0);
                    printf("Done, sign in success\n");
                    return;
                }
                
            }
            user = user->next;
        }

        // if not exist
        char *msg = "This Account not exist!\n";
        // send(conn_sock, msg, strlen(msg), 0);
        printf("Account \"%s\" is not exist.\n", name);
        return;
           
    } while (1);
}

// int main() {
//     signUp("test", "123");
//     signUp("hello", "123");
//     signUp("sig nupok", "123");
//     updateListUser();
//     logIn("abc", "123");
// }