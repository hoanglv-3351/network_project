#include "user.h"

#include <string.h>
#include <stdlib.h>

#include "signal.h"

User *createNewUser(int ID, char username[], char password[])
{
    User *new = (User *)malloc(sizeof(User));
    new->ID = ID;
    strcpy(new->name, username);
    strcpy(new->password, password);
    return new;
}

void insertUser(User *root, int ID, char username[], char password[])
{
    User *new = createNewUser(ID, username, password);
    if (root == NULL)
    {
        new->next = root;
        root = new;
    }
    else
    {
        User *p = root;
        //tro con tro toi cuoi danh sach lien ket
        while (p->next != NULL)
            p = p->next;
        p->next = new;
    }
}

User *readUserFile(char filename[])
{
    User *root;

    int ID;
    char name[32], password[32];

    FILE *f;

    //strcpy(filename, "db/users.txt")
    if (!(f = fopen(filename, "r")))
    {
        printf("\nCreate User Database failed! File not found.\n");
    }
    else
    {
        int number_of_users = 0;
        int check = 1;
        fscanf(f, "%d\n", &number_of_users);
        if (number_of_users == 0)
            return NULL;
        while (!feof(f))
        {
            fscanf(f, "%d\n%s\n%s\n", &ID, name, password);

            if (check == 1)
            {
                root = createNewUser(ID, name, password);
                check = 0;
            }
            else
                insertUser(root, ID, name, password);
        }
    }
    fclose(f);
    return root;
}

User *searchUserByUsername(User *root, char name[])
{
    User *p = root;
    while (p != NULL)
    {
        if (strcmp(name, p->name) == 0)
        {
            User *tmp = createNewUser(p->ID, p->name, p->password);
            return tmp;
        }
        p = p->next;
    }
    return NULL; // 0 is not exist
}
void printAllUser(User *root)
{
    User *p = root;
    while (p != NULL)
    {
        printf("%s\n", p->name);
        p = p->next;
    }
}

User *searchUserByID(User *root, int ID)
{
    User *p = root;
    while (p != NULL)
    {
        if (p->ID == ID)
        {
            User *tmp = createNewUser(p->ID, p->name, p->password);
            return tmp;
        }
        p = p->next;
    }
    return NULL;
}

char *verifyAccount(User *root, char *name, char *password, int *flag)
{
    static char response[64];

    User *p = searchUserByUsername(root, name);
    //check if username is exist or not
    if (p != NULL)
    {
        // check password
        if (strcmp(password, p->password) != 0)
        {
        
            strcpy(response, MESS_RETRY_PASSWORD);
            return response;
        }
        // login success
        else
        {
            *flag = 1;
            strcpy(response, MESS_LOGIN_SUCCESS);
            return response;
        }
    }

    strcpy(response, MESS_ACCOUNT_NOT_EXIST);
    return response;
}

void freeUserData(User *root)
{
    /* deref head_ref to get the real head */
    User *tmp;
    while (root != NULL)
    {
        tmp = root->next;
        printf("Delete %s\n", root->name);
        free(root);
        root = tmp;
    }
}