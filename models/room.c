#include<stdio.h>
#include<stdlib.h>
#include <string.h>


#include "room.h"


Room * createNewRoom(int ID, char name[], int wsp_id)
{
    Room *new = (Room *)malloc(sizeof(Room));

    new->ID = ID;
    strcpy(new->name, name);
    new->num_of_users = 0;

    // read Each WorkSpace users
    char filename[32];
    strcpy(filename,"db/room_users_");
    char tmp[10];
    sprintf(tmp, "%d_", wsp_id);
    strcat(filename, tmp);
    sprintf(tmp, "%d.txt", new->ID);
    strcat(filename, tmp);

    FILE *f;
    if (!(f = fopen(filename, "r")))
    {
        printf("\nCreate Room Database failed! File Users not found.\n");
    }
    else
    {
        
        while (!feof(f))
        {
            
            fscanf(f, "%d", &new->user_id[new->num_of_users]);
            new->num_of_users += 1;
        }
    }
    

    return new;
}
// void insertRoom(Room *root, char name[])
// {
//     Room *new = createNewRoom(0, name);
//     if (root == NULL)
//     {
//         new->next = root;
//         root = new;
//     }
//     else
//     {
//         Room *p = root;
//         while (p->next != NULL)
//             p = p->next;
//         new->ID = p->ID + 2;
//         p->next = new;
//     }
// }

