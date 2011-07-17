#include <stdlib.h>
#include <stdio.h>
#include "bp.h"
#include "gui.h"
#include "list.h"

int main(int argc, char *argv[]) 
{	
	// Make sure we have enough arguments
	// This could be made alot more robust
	if(argc < 5)
	{
		fprintf(stderr, "Usage: %s <source_eid> <dest_eid> <nick> <max_delay>\n", argv[0]);
		fprintf(stderr, "\tsource_eid - the eid the client sends from\n");
		fprintf(stderr, "\tdest_eid - the eid the client sends to\n");
		fprintf(stderr, "\tnick - nickname for the chat\n");
		fprintf(stderr, "\tmax_delay - the maximum delay in seconds between client and server\n");
		return 1;
	}

	setNick(argv[3]);

	// ======================= set up the SBP stuff ====================
	bpInit(argv[1], argv[2], atoi(argv[4]));
	
	// ======================= lets make pretty stuff ====================
    init();
    run();
    
    // ======================= stop the SBP stuff ====================
    bpQuit();
	
    return 0;
}
