#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sbp_api.h>

int main(int argc, char *argv[]) 
{
	// parse params
	//...
	char src_eid[] = "ipn:1.1";
	char dst_eid[] = "ipn:1.2";
	
	// ======================= set up the SBP stuff ====================
	SBP_Init();
	SBP_Connection *conn = SBP_CreateConnection(src_eid);
	if(conn == NULL)
	{
		SBP_FATAL("Unable to open connection. . .");
	}
	
	conn->blocking = SBP_POLL;
	SBP_SetDestination(conn, dst_eid);
	
	bp_init(conn);
	
	// ======================= lets make pretty stuff ====================
    init();
    run();
    
    // ======================= stop the SBP stuff ====================
    SBP_DestroyConnection(conn);
	SBP_Shutdown();
	
    return 0;
}
