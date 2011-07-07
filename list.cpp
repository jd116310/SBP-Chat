#include <string.h>
#include <vector>
#include "list.h"
#include "gui.h"
#include "bp.h"

using namespace std;

static int counter = 0;
vector<item> qlist;
vector<item> blist;

void sendMessage(char *p)
{
	item i;
	i.id = counter++;
	strcpy(i.buff, p);
	
	qlist.push_back(i);
	
	bpSendMessage(p);
	gui_update(&qlist, &blist);
}

void recvMessage(char *p)
{
	/*item i;
	i.id = counter++;
	strcpy(i.buff, p);
	blist.push_back(i);*/
	//removeitem(&qlist, 0);
	gui_update(&qlist, &blist);
}

