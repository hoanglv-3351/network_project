#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "signal.h"
#include "message.h"
#include "utils.h"

Message *createNewMess(int parent_id, char time[], int send_id,  char content[])
{
    Message *new = (Message *)malloc(sizeof(Message));
    new->ID = 1;
    new->parent_id = parent_id;
    new->send_id = send_id;
    new->datetime = convertStringToTimeT(time);
    strcpy(new->content, content);
    return new;
}

Message * insertMess(Message *root, int parent_id, char time[], int send_id, char content[])
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
    return root;
}

char *createMessFilename(int wsp_id, int room_id)
{
    char * filename = (char*) malloc(sizeof(char));
    strcpy(filename, "db/message_");
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
        printf("-1\n");
        printf("The two of you have connected and can start talking to each other.\n");
        return NULL;
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
            //printf("2\n");
            
            fscanf(f, "\n%d\n%d\n%s\n%d\n", &ID, &parent_id, time, &send_id);
            fscanf(f, "%[^\n]", content);
            if (feof(f))
                break;

            //printf("%d\n%d\n%s\n%d\n%d\n%s\n", ID, parent_id, time, send_id,  content);
            if (check == 1)
            {
                //printf("3\n");
                root = createNewMess(parent_id, time, send_id, content);
                check = 0;
            }
            else
            {
                //printf("4\n");
                root = insertMess(root, parent_id, time, send_id,  content);
            }
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
    
    // Message *tmp;
    // while (root != NULL)
    // {
    //     tmp = root;
    //     root = root->next;
    //     //printf("Delete mess id=%d\n", root->ID);
    //     free(tmp);
    // }
    Message * current = root;
    Message * next;
 
   while (current != NULL)
   {
       next = current->next;
       free(current);
       current = next;
   }
   root = NULL;
    
}


void writeMessData(Message *root, int wsp_id, int room_id)
{
    char filename[32];
    strcpy(filename, createMessFilename(wsp_id, room_id));
    printf("Write file %s\n", filename);

    FILE *f;
    if (!(f = fopen(filename, "w")))
    {
        printf("\nFile Message Database %s not found.\n", filename);
    }
    else
    {
        Message *p = root;
        while (p != NULL)
        {

            fprintf(f, "%d\n", p->ID);
            fprintf(f, "%d\n", p->parent_id);
            fprintf(f, "%s\n", convertTimeTtoString(p->datetime,2));
            fprintf(f, "%d\n", p->send_id);
            fprintf(f, "%s\n", p->content);
            p = p->next;
        }
    }
    fclose(f);
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
Message *findLastMess(Message *root)
{
    Message *p = root;
    while (p->next != NULL)
    {
        p = p->next;
    }
    return p;
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