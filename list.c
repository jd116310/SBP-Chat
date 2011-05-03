#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "list.h"

static int counter = 0;

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
			p->next == i->next->next;
			free(i);
			return;
		}
	}
	
	//um...it wasnt found
	return; //error
}

int len(item *head)
{
	int i;
	item *p;
	for(p = head, i = 0; p != NULL; p = p->next, ++i);
	
	return i;
}

void displayList(item *head, WINDOW* w)
{
	item *i;
	int line = 1;
		
	for(i = head; i != NULL; i=i->next, ++line)
	{
		// clear line
		wmove(w, line, 0);
		wclrtoeol(w);
		
		// add tick mark
		mvwaddch(w, line, 0, '-');
		
		// put string
		mvwprintw(w, line, 1, i->buff);
	}
	wrefresh(w);
}

