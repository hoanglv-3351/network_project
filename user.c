#include "user.h"

#include <string.h>
#include <stdlib.h>



User * createNewUser(int ID, char username[], char password[])
{
    User *new = (User *)malloc(sizeof(User));
    new->ID = ID;
    strcpy(new->name,username);
    strcpy(new->password, password);
    return new;

}

void insertUser(User * root, int ID, char username[], char password[])
{
  User * new = createNewUser(ID, username, password);
  if(root==NULL)
    {
      new->next=root;
      root=new;
    }
  else
    {
      User *p=root;
      //tro con tro toi cuoi danh sach lien ket
      while(p->next!=NULL) p=p->next; 
      p->next=new;
    }
}

User * readUserFile(char filename[])
{
    User *root;

    int ID;
    char name[32], password[32];
    
    FILE *f;

    //strcpy(filename, "db/users.txt")
    if (!(f = fopen(filename,  "r")))
    {
        printf("\nCreate User Database failed! File not found.\n\n");
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
                insertUser(root , ID, name, password);
        }
    }
    fclose(f);
    return root;
}



User * searchUserByUsername(User * root, char name[])
{
    
    User *p =root;
    while (p != NULL)
    {
        if (strcmp(name, p->name) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return NULL; // 0 is not exist
}

User *searchUserByID(User * root, int ID)
{
    User *p = root;
    while (p != NULL)
    {
        if (p->ID == ID)
            return p;
        p = p->next;
    }
    return NULL;
}

// int main(){
//     User *userList = updateListUser();
//     User *p = headUser;
//     while(p != NULL) {
//         printf("%d --- %s --- %s\n", p->ID, p->name, p->password);
//         p = p->next;
//     }
//     User *kq = findUserByID(1);
//     if (kq != NULL) printf("%s", kq->name);
// }

// extern int toupper(int __c) __THROW;

// int isValid(char *str)
// {
//     int valid = 1;
//     int idx = 0;
//     for (idx; idx < strlen(str); idx++)
//     {
//         char c = (char)toupper(str[idx]);
//         if ((c <= 90 && c >= 65) || (c >= 48 && c <= 57))
//             continue;
//         else
//         {
//             valid = 0;
//             break;
//         }
//     }
//     return valid;
// }

// void signUp(int conn_sock, char *name, char *password)
// {
//     updateListUser(); //
//     char *msg;

//     if (!isValid(name) || !isValid(password))
//     {
//         msg = "Invalid name or password!\n";
//         send(conn_sock, msg, strlen(msg), 0);
//         printf("%s", msg);
//         return;
//     }

//     if (isExistingUser(name))
//     {
//         msg = "This user is already exist!\n";
//         send(conn_sock, msg, strlen(msg), 0);
//         printf("%s", msg);
//         return;
//     }
//     else
//     {
//         number_of_users++;
//         FILE *f;
//         f = fopen("db/users.txt", "w");
//         fprintf(f, "%d", number_of_users);
//         User *user = (User *)malloc(sizeof(User));
//         user->ID = number_of_users;
//         strcpy(user->name, name);
//         strcpy(user->password, password);
//         insertUser(user);

//         User *ptr = headUser;
//         while (ptr != NULL)
//         {
//             fprintf(f, "\n%d\n%s\n%s", ptr->ID, ptr->name, ptr->password);
//             ptr = ptr->next;
//         }
//         free(ptr);
//         fclose(f);
//         msg = "Create new account successfully !\n";
//         send(conn_sock, msg, strlen(msg), 0);
//         printf("%s", msg);
//     }
// }

char *verifyAccount(User * root, char *name, char *password, int * flag)
{
    static char response[32];

    User *p = searchUserByUsername(root, name);
        //check if username is exist or not
        if (p != NULL)
        {
            // check password
            if (strcmp(password, p->password) != 0)
            {
                *flag = 1;
                strcpy(response, "Password is incorrect.\nEnter again: ");
                return response;
            }
            // login success
            else
            {
                *flag = 0;
                strcpy(response, "Login successfully.\n");
                return response;
            }
        }
    
    *flag=2;
    strcpy(response, "This account not exist!\nEnter again: ");
    return response;
}