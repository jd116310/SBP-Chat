#ifndef GUI_H
#define GUI_H

#include "list.h"

// intialize ncurses and get everything set up
void init();

// runs in the main loop
void run();

// when the "model" has changed, update the "view"
void gui_update(item *q, item *b);

#endif

