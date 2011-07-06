#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "list.h"
#include "gui.h"
#include "bp.h"

static int counter = 0;
item *qlist = NULL; // queue list
item *blist = NULL; // board list

void additem(item **head, const char *p)
{
	item *n = (item *)malloc(sizeof(item));
	item *i;
	
	n->id = counter++;
	strcpy(n->buff, p);
	n->next = NULL;
	
	if(*head == NULL)
	{
		*head = n;
	}
	else
	{
		for(i = *head; i->next != NULL; i=i->next);
		
		i->next = n;
	}
	return;
}

void removeitem(item **head, int id)
{
	item *i, *p;
	
	if((*head != NULL) && (id == (*head)->id))
	{
		i = *head;
		(*head) = (*head)->next;
		free(i);
		return;
	}
	
	for(i = *head; i != NULL; p=i, i=i->next)
	{
		if(id == i->id)
		{
			p->next = i->next;
			free(i);
			return;
		}
	}
	
	//um...it wasnt found
	return; //error
}

// length of a list
int len(item *head)
{
	int i;
	item *p;
	for(p = head, i = 0; p != NULL; p = p->next, ++i);
	
	return i;
}

// Adds a message to the queue, sends it over BP, and updates the gui
void sendMessage(char *p)
{
	additem(&qlist, p);
	bpSendMessage(p);
	gui_update(qlist, blist);
}

void recvMessage(char *p)
{
	additem(&blist, p);
	//removeitem(&qlist, 0);
	gui_update(qlist, blist);
}

