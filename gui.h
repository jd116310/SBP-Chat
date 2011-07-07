#ifndef GUI_H
#define GUI_H

#include <vector>
#include "list.h"

using namespace std;

// intialize ncurses and get everything set up
void init();

// runs in the main loop
void run();

// when the "model" has changed, update the "view"
void gui_update(vector<item> *q, vector<item> *b);

#endif

