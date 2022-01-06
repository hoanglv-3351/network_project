#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "notice.h"

Notice *createNewNotice(char content[], int is_read)
{
    Notice *new = (Notice *)malloc(sizeof(Notice));
    new->is_read = is_read;
    strcpy(new->content,content);
    return new;
}
Notice *insertNotice(Notice *root, char content[], int is_read)
{
    Notice *new = createNewNotice(content, is_read);
    if (root == NULL)
    {
        new->next = root;
        root = new;
    }
    else
    {
        Notice *p = root;
        //tro con tro toi cuoi danh sach lien ket
        while (p->next != NULL)
            p = p->next;
        p->next = new;
    }
    return root;
}
Notice *readAllNoticeData(int user_id)
{
    Notice *root = NULL;
    int is_read;
    char content[128];

    char filename[32];
    strcpy(filename, "db/user_notices_");
    char tmp[10];
    sprintf(tmp, "%d.txt", user_id);
    strcat(filename, tmp);
    FILE *f;
    if (!(f = fopen(filename, "r")))
    {
        printf("Can't load Notice Database.\n");
        return NULL;
    }
    else
    {
        int check = 1; 
        while (!feof(f))
        {
            fscanf(f, "\n%d\n", &is_read);
            fscanf(f, "%[^\n]", content);
            // if (feof(f))
            //     break;
            if (check == 1)
            {
                root = createNewNotice(content, is_read);
                check = 0;
            }
            else
            {
                root = insertNotice(root, content, is_read);
            }
        }
    }
    fclose(f);
    return root;
}
Notice *readNoticeData(int user_id)
{
    Notice *root = NULL;
    char content[128];

    char filename[32];
    strcpy(filename, "db/user_notices_");
    char tmp[10];
    sprintf(tmp, "%d.txt", user_id);
    strcat(filename, tmp);
    FILE *f;
    
    if (!(f = fopen(filename, "r")))
    {
        printf("Can't load Notice Database.\n");
        return NULL;
    }
    else
    {
        int check = 1;
        
        while (!feof(f))
        {
            int is_read = 0;
            fscanf(f, "%d\n", &is_read);
            fscanf(f, "%[^\n]\n", content);
            if (feof(f))
                break;
            if (is_read == 0)
            {
                if (check == 1)
                {
                    root = createNewNotice(content, is_read);
                    check = 0;
                }
                else
                {
                    root = insertNotice(root, content, is_read);
                }
            }
            //printf("%d\n%s\n",is_read, content);
        }
    }
    fclose(f);
    return root;
}

void saveNotice(int user_id, char content[])
{
    char filename[32];
    strcpy(filename, "db/user_notices_");
    char tmp[10];
    sprintf(tmp, "%d.txt", user_id);
    strcat(filename, tmp);
    FILE *f;
    if (!(f = fopen(filename, "a")))
    {
        printf("Can't load Notice Database.\n");
        return;
    }
    else
    {
        fprintf(f, "%d\n%s\n", 0, content);
    }
    fclose(f);
}
void RewriteNoticeData(int user_id)
{
    Notice *root = readAllNoticeData(user_id);
    Notice *p = root;

    char filename[32];
    strcpy(filename, "db/user_notices_");
    char tmp[10];
    sprintf(tmp, "%d.txt", user_id);
    strcat(filename, tmp);
    FILE *f;
    if (!(f = fopen(filename, "w")))
    {
        printf("Can't load Notice Database.\n");
        return;
    }
    else
    {
        while (p != NULL)
        {
            fprintf(f, "%d\n%s\n", 1, p->content);
            p = p->next;
        }
    }
    fclose(f);
}