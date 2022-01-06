#ifndef _NOTICE_H_
#define _NOTICE_H_
typedef struct Notice{
    char content[64];
    int is_read; //if read = 1, not read = 0
    time_t datetime;
    struct Notice *next;
} Notice;


Notice *createNewNotice(char content[], int is_read);
Notice *insertNotice(Notice *root, char content[], int is_read);

Notice *readAllNoticeData(int user_id);
Notice *readNoticeData(int user_id);
void saveNotice(int user_id, char content[]);
void RewriteNoticeData(int user_id);
#endif