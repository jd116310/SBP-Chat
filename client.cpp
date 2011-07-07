#include "bp.h"
#include "gui.h"

int main(int argc, char *argv[]) 
{
	// parse params
	//...
	char src_eid[] = "ipn:1.1";
	char dst_eid[] = "ipn:1.2";
	
	// ======================= set up the SBP stuff ====================
	bpInit(src_eid, dst_eid);
	
	// ======================= lets make pretty stuff ====================
    init();
    run();
    
    // ======================= stop the SBP stuff ====================
    bpQuit();
	
    return 0;
}
