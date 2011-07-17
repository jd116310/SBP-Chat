#ifndef LIST_H
#define LIST_H

#include <time.h>

#define TIME_STR_LEN 9

typedef struct item
{
	int userid;
	int messageid;
	char nick[16];
	time_t timestamp;
	char buff[256];
}item;

void getTime(char *buff, time_t rawtime);
void setNick(char *n);

// Simple enough...
void sendMessage(char *p);
void recvMessage(void *p);

#endif

