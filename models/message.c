#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "signal.h"
#include "message.h"
#include "utils.h"

Message *createNewMess(int parent_id, char time[], int send_id,  char content[])
{
    Message *new = (Message *)malloc(sizeof(Message));
    new->parent_id = parent_id;
    new->send_id = send_id;
    new->datetime = convertStringToTimeT(time);
    strcpy(new->content, content);
    return new;
}

void insertMess(Message *root, int parent_id, char time[], int send_id, char content[])
{
    Message *new = createNewMess(parent_id, time, send_id,  content);
    if (root == NULL)
    {
        new->next = root;
        root = new;
    }
    else
    {
        Message *p = root;
        //tro con tro toi cuoi danh sach lien ket
        while (p->next != NULL)
            p = p->next;

        new->ID = p->ID + 1;
        p->next = new;
    }
}

char *createMessFilename(int wsp_id, int room_id)
{
    static char filename[32] = "db/message_";
    char tmp[16];
    sprintf(tmp, "%d_", wsp_id);
    strcat(filename, tmp);
    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "%d.txt", room_id);
    strcat(filename, tmp);

    return filename;
}
Message *readMessData(char filename[])
{
    Message *root;
    FILE *f;
    int ID;
    char content[MESS_LEN];
    char time[64];
    int parent_id, send_id;

    //strcpy(filename, "db/users.txt")
    if (!(f = fopen(filename, "r")))
    {
        printf("The two of you have connected and can start talking to each other.\n");
    }
    else
    {

        //int number_of_messes = 0;
        int check = 1;
        //fscanf(f, "%d", &number_of_messes);
        // if (number_of_messes == 0)
        //     return NULL;

        while (!feof(f))
        {

            fscanf(f, "\n%d\n%d\n%s\n%d\n", &ID, &parent_id, time, &send_id);
            fscanf(f, "%[^\n]", content);

            //printf("%d\n%d\n%s\n%d\n%d\n%s\n", ID, parent_id, time, send_id,  content);
            if (check == 1)
            {
                root = createNewMess(parent_id, time, send_id, content);
                root->ID = 1;
                check = 0;
            }
            else
            {
                insertMess(root, parent_id, time, send_id,  content);
            }
            if (feof(f))
                break;
            //printAllMess(root);
        }
    }
    fclose(f);
    return root;
}

void printAllMess(Message *root)
{
    Message *p = root;
    while (p != NULL)
    {
        printf("(%d) %s\n", p->ID, p->content);
        p = p->next;
    }
}
void freeMessData(Message *root)
{
    /* deref head_ref to get the real head */
    Message *tmp;
    while (root != NULL)
    {
        tmp = root->next;
        printf("Delete mess id=%d\n", root->ID);
        free(root);
        root = tmp;
    }
}

void writeMessData(Message *root, int wsp_id, int room_id)
{
    char filename[16];
    strcpy(filename, createMessFilename(wsp_id, room_id));
    printf("Write file %s", filename);

    FILE *f;
    if (!(f = fopen(filename, "w")))
    {
        printf("\nFile Message Database %s not found.\n", filename);
    }
    else
    {
        // Message *p = root;
        // while (p != NULL)
        // {

        //     fprintf(f, "%d\n", p->ID);
        //     fprintf(f, "%d\n", p->parent_id);
        //     fprintf(f, "%s\n", convertTimeTtoString(p->datetime,0));
        //     fprintf(f, "%d\n", p->send_id);
        //     fprintf(f, "%s\n", p->content);
        //     p = p->next;
        // }
    }
}

Message *findMessByID(Message *root, int ID)
{
    Message *p = root;
    while (p != NULL)
    {
        if (p->ID == ID)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}
// Message *findMessFromDate(time_t time)
// {
//      Message * new;
//     return new;

// }
// Message *findMessFromToDate(time_t from_time, time_t to_time)
// {
//      Message * new;
//     return new;

// }
// Message *findMessInDate(time_t time)
// { Message * new;
//     return new;

// }