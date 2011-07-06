#ifndef BP_H
#define BP_H

#include <sbp_api.h>

void bpInit(char *src, char *dst);
void bpQuit();
void bpSendMessage(char *m);
//void bp_recv();

#endif

