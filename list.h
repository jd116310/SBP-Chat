#ifndef LIST_H
#define LIST_H

typedef struct item
{
	int id;
	char buff[256];
	struct item *next;
}item;

// Simple enough...
void sendMessage(char *p);
void recvMessage(char *p);

//void additem(item **head, const char *p);
//void removeitem(item **head, int id);
//int len(item *head);

#endif

