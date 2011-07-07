#include <string.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "gui.h"
#include "bp.h"

using namespace std;

static int userid = 0;
vector<item> qlist;
vector<item> blist;

void sendMessage(char *p)
{
	item i;
	
	if(userid == 0)
	{
		srand(time(NULL));
		userid = rand();	
	}
	i.userid = userid;
	i.messageid = rand();
	
	strcpy(i.buff, p);
	
	qlist.push_back(i);
	
	bpSendMessage(&i, sizeof(item));
	gui_update(&qlist, &blist);
}

void recvMessage(void *p)
{
	unsigned int i;
	int found = 0;
	item message;

	memcpy(&message, p, sizeof(item));
	
	for(i = 0; i < qlist.size(); ++i)
	{
		if(message.userid == userid && message.messageid == qlist[i].messageid)
		{
			vector<item>::iterator nth = qlist.begin() + i;
			blist.push_back(*nth);
			qlist.erase(nth);
			found = 1;
			break;
		}
	}
	
	if(!found)
	{
		blist.push_back(message);
	}
	
	gui_update(&qlist, &blist);
}

