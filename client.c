#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sbp_api.h>

int main(int argc, char *argv[]) 
{
	// parse params
	//...
	char src_eid[] = "ipn:1.1";
	char dst_eid[] = "ipn:1.2";
	
	SBP_Init();
	SBP_Connection *conn = SBP_CreateConnection(src_eid);
	SBP_SetDestination(conn, dst_eid);
	
	//conn->blocking = SBP_BLOCKING;
	//int err = conn->listen(conn);

	// Initialize the gui
    init();
    
    // run the gui's main loop
    run();
    
    SBP_DestroyConnection(conn);
	SBP_Shutdown();
	

    return EXIT_SUCCESS;
}

/*

char * intprtkey(int ch) {

    /*  Define a selection of keys we will handle

    static struct keydesc keys[] = { { KEY_UP,        "Up arrow"        },
				     { KEY_DOWN,      "Down arrow"      },
				     { KEY_LEFT,      "Left arrow"      },
				     { KEY_RIGHT,     "Right arrow"     },
				     { KEY_HOME,      "Home"            },
				     { KEY_END,       "End"             },
				     { KEY_BACKSPACE, "Backspace"       },
				     { KEY_IC,        "Insert"          },
				     { KEY_DC,        "Delete"          },
				     { KEY_NPAGE,     "Page down"       },
				     { KEY_PPAGE,     "Page up"         },
				     { KEY_F(1),      "Function key 1"  },
				     { KEY_F(2),      "Function key 2"  },
				     { KEY_F(3),      "Function key 3"  },
				     { KEY_F(4),      "Function key 4"  },
				     { KEY_F(5),      "Function key 5"  },
				     { KEY_F(6),      "Function key 6"  },
				     { KEY_F(7),      "Function key 7"  },
				     { KEY_F(8),      "Function key 8"  },
				     { KEY_F(9),      "Function key 9"  },
				     { KEY_F(10),     "Function key 10" },
				     { KEY_F(11),     "Function key 11" },
				     { KEY_F(12),     "Function key 12" },
				     { -1,            "<unsupported>"   }
    };
    static char keych[2] = {0};
    
    if ( isprint(ch) && !(ch & KEY_CODE_YES)) 
    {
		/*  If a printable character 

		keych[0] = ch;
		return keych;
    }
    else 
    {
		/*  Non-printable, so loop through our array of structs 
		int n = 0;
	
		do {
			if ( keys[n].code == ch )
			return keys[n].name;
			n++;
		} while ( keys[n].code != -1 );

		return keys[n].name;
    }    
    
    return NULL;        /*  We shouldn't get here
}
*/
