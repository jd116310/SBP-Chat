#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include "list.h"

item *qlist = NULL; // queue list
item *blist = NULL; // board list

typedef struct win
{
	int h;	// heigth of the window
	int w;	// width of the window
	int y;	// the top left y position
	int x;	// the top left x position
	WINDOW *window;	// the window pointer
}win;

win input;	// people type here
win queue;	// displays messages that are queued before getting placed on the board
win board;	// posted messages go here


void createHeader(win w, const char *s) 
{
	int i;
	
	wmove(w.window, 0 ,0);
    waddch(w.window, ACS_HLINE);
    wprintw(w.window, s);
	for(i = strlen(s) + 1; i < w.w; ++i)
		waddch(w.window, ACS_HLINE);
	
	wrefresh(w.window);
}

int main(void) 
{
    char buff[256];
    int ch;
    int x, y, i;
    int curline = 1;
    

    //  Initialize ncurses
    if ((initscr()) == NULL ) 
    {
		fprintf(stderr, "Error initializing ncurses.\n");
		exit(EXIT_FAILURE);
    }
    
	// Set up ncurses
	cbreak();
	nonl();
    noecho();				// Turn off key echoing
    keypad(stdscr, TRUE);	// Enable the keypad for non-char keys  */
    
    // this is important i think
	refresh();
	
	// ======================= set up the input window ====================
	
	input.h = 2;
	input.w = COLS;
	input.y = LINES - input.h;
	input.x = 0;
    input.window = newwin(input.h, input.w, input.y, input.x);
    
    // create a horizontal line
    createHeader(input, "Message");
    
    // ======================= set up the queue window ====================
    
    queue.h = 6;
	queue.w = COLS;
	queue.y = LINES - queue.h - input.h;
	queue.x = 0;
    queue.window = newwin(queue.h, queue.w, queue.y, queue.x);
    
    createHeader(queue, "Queue");
	
	// ======================= set up the board window ====================
	
	board.h = LINES - queue.h - input.h;
	board.w = COLS;
	board.y = 0;
	board.x = 0;
    board.window = newwin(board.h, board.w, board.y, board.x);
    
    // set scroll region, dont know what they do...
	//scrollok(board.window, TRUE);
	//idlok(board.window, TRUE);
	
	createHeader(board, "Chat");
	
	// draw stuff
	wrefresh(input.window);
    
    // ======================= main event loop ====================
	
    //  Loop until user presses 'q'
    while ( (ch = getch()) != 'q' ) 
    {
    	x = getcurx(input.window);
    	switch(ch)
    	{
    		case KEY_PPAGE: // page up
    		case KEY_NPAGE: // page down
    			// scroll board down
    			break;
    		case KEY_BACKSPACE:
    			if(x == 0) break;
    			wmove(input.window, 1, x-1);
				wdelch(input.window); 
				break;
			case KEY_DC:
				wdelch(input.window); 
				break;
			case KEY_LEFT:
    			wmove(input.window, 1, x-1);
    			break;
			case KEY_RIGHT:
				wmove(input.window, 1, x+1);
				break;
			case 0xd: // Enter
				//wscrl(input.window, 1);
				
				// get the message from the input
				mvwinnstr(input.window, 1, 0, buff, 256);
				
				// put it on the queue's list
				additem(&qlist, buff);
				
				if(len(qlist) > 3)
				{
					additem(&blist, qlist->buff);
					removeitem(&qlist, qlist->id);
				}
				
				// display the list
				displayList(blist, board.window);
				displayList(qlist, queue.window);
    			
    			// clear input
    			wmove(input.window, 1 , 0);
    			wclrtoeol(input.window);
    			
    			break;
    		default:
    			if(x == COLS) break;
				winsch(input.window, ch);
				wmove(input.window, curline, x+1);
				break;
        }
        
        // refresh
		wrefresh(input.window);
    }
    
    //  Clean up after ourselves
    delwin(input.window);
    delwin(queue.window);
    delwin(board.window);
    endwin();

    return EXIT_SUCCESS;
}

/*  Struct to hold keycode/keyname information  

struct keydesc {
    int  code;
    char name[20];
};


/*  Returns a string describing a character passed to it

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
