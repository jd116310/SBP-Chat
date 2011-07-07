#ifndef LIST_H
#define LIST_H

typedef struct item
{
	int id;
	//int userid;
	//int date;
	char buff[256];
}item;

// Simple enough...
void sendMessage(char *p);
void recvMessage(char *p);

#endif

