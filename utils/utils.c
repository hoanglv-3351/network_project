#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
// #include <sys/socket.h>
// #include <sys/types.h>

#include <netinet/in.h>

#include "utils.h"

void print_ip_addr(struct sockaddr_in addr)
{
  printf("%d.%d.%d.%d",
         addr.sin_addr.s_addr & 0xff,
         (addr.sin_addr.s_addr & 0xff00) >> 8,
         (addr.sin_addr.s_addr & 0xff0000) >> 16,
         (addr.sin_addr.s_addr & 0xff000000) >> 24);
}
time_t convertStringToTimeT(char time[])
{
  int d, m, y, hh, mm, ss;
  struct tm when;
  if (strlen(time) <=  11)
  {
    sscanf(time, "%d/%d/%d", &d, &m, &y);
    hh = 0;
    mm = 0;
    ss = 0;
  }
  else
    sscanf(time, "%d/%d/%d-%d:%d:%d", &d, &m, &y, &hh, &mm, &ss);

  when.tm_mday = d;
  when.tm_mon = m - 1;
  when.tm_year = y - 1900;
  when.tm_hour = hh;
  when.tm_min = mm;
  when.tm_sec = ss;

  time_t converted;
  converted = mktime(&when);

  return converted;
}
/*
 * Function:  convertTimeTtoString
 * --------------------
 * convert time_t to string to print
 *
 *  time_t: time
 *  type: 0 ex: (Wednesday, Dec 15 2021) 08:27:00 AM
 *        1 ex: 10/12/2021 09:20
 *
 *  returns: string 
 */
char *convertTimeTtoString(time_t converted, int type)
{
  struct tm *timeptr;

  timeptr = localtime(&converted);
  

  static char time_str[64];

  if (type == 0)
  {
    strftime(time_str, sizeof(time_str), "(%A, %b %d %G) %r", timeptr);
  }

  else if (type == 1)
  {
    strftime(time_str, sizeof(time_str), "%d/%m/%Y %R", timeptr);
  }
  else if (type == 2) //infile
  {
    strftime(time_str, sizeof(time_str), "%d/%m/%Y-%T", timeptr);
  }

  //printf("Current Time : %s\n", time_str);
  return time_str;
}

char *getCurrentTime(int type)
{
  static char timestr[64];
  time_t mytime = time(NULL);

  strcpy(timestr, convertTimeTtoString(mytime, type));

  return timestr;
}

int createFakeRoom(int a, int b)
{
  // char s1[3];
  // char s2[3];
  // if (a < b)
  // {
  // 	sprintf(s1, "%d", a*10);
  // 	sprintf(s2, "%d", b);
  // }
  // else
  // {
  // 	sprintf(s1, "%d", b*10);
  // 	sprintf(s2, "%d", a);
  // }
  // strcat(s1, s2);
  // int c = atoi(s1);

  int c = 0;
  if (a < b)
  {
    c = pow(5, a) * pow(3, b);
  }
  else
  {
    c = pow(3, a) * pow(5, b);
  }

  

  return c;
}

int returnFakeRoomToID(int c, int a)
{
  int b = 0;
  int count_3 = 0;
  int count_5 = 0;
  while (c > 1)
  {
    if (c % 3 == 0)
    {
      count_3 += 1;
      c = c / 3;
    }
    if (c % 5 == 0)
    {
      count_5 += 1;
      c = c / 5;
    }
  }
  b = (a == count_3) ? count_5 : count_3;
  return b;
}


char ** splitString(char str[], char newString[][16], int * num_word)
{
  int j=0; 
  int ctr =0;
    for(int i=0;i<=(strlen(str));i++)
    {
      
        // if space or NULL found, assign NULL into newString[ctr]
        if(str[i]==' '||str[i]=='\0')
        {
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            newString[ctr][j]=str[i];
            j++;
        }
    }
    *num_word = ctr;
    return newString;
    
}

char ** splitStringByLine(char str[], char newString[][128], int * num_word)
{
  int j=0; 
  int ctr =0;
    for(int i=0;i<=(strlen(str));i++)
    {
      
        // if space or NULL found, assign NULL into newString[ctr]
        if(str[i]=='\n'||str[i]=='\0')
        {
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            newString[ctr][j]=str[i];
            j++;
        }
    }
    *num_word = ctr;
    return newString;
    
}
int valueInArray(int val, int arr[], int size)
{

    for (int i = 0; i < size; i++)
    {
        if (arr[i] == val)
            return 1;
    }
    return 0;
}


// char * getFirstLine(char str[], char * newString[])
// {
//   int j=0, i = 0; 
//   int ctr =0;
//     for( i=0;i<=(strlen(str));i++)
//     {   

//         // if space or NULL found, assign NULL into newString[ctr]
//         if(str[i]=='\n'||str[i]=='\0')
//         {
//             newString[0][j]='\0';
//             j = 0;
//             break;
//         }
//         else
//         {
//             newString[0][j]=str[i];
//             j++;
//         }
//     }
//     for(int k=i;k<=(strlen(str));j++,k++)
//     {   
//       newString[1][j]=str[k];
//     }
//     *num_word = ctr;
//     return newString;
    
// }