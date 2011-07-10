#ifndef LIST_H
#define LIST_H

#define TIME_STR_LEN 9

typedef struct item
{
	int messageid;
	int userid;
	char time[TIME_STR_LEN];
	char buff[256];
}item;

// Simple enough...
void sendMessage(char *p);
void recvMessage(void *p);

#endif

