#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
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

char *trim(char *c) 
{
    char *e = c + strlen(c) - 1;
    while(*c && isspace(*c)) c++;
    while(e > c && isspace(*e)) *e-- = '\0';
    return c;
}

// Puts a nice header at the top of a window in the form:
// -mystring--------------------------------------------
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

void init()
{
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
}

void run()
{
	int ch;
    int x, y, i;
    int curline = 1;
	char buff[256];
	char *message;
	
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
				message = trim(buff);
				
				if(strlen(message) == 0) break;
				//strcat(message, "!");
				
				// put it on the queue's list
				additem(&qlist, message);
				
				// temporary for testing
				if(len(qlist) > 3)
				{
					additem(&blist, qlist->buff);
					removeitem(&qlist, qlist->id);
				}
				
				// display the lists
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
}
