#ifndef BP_H
#define BP_H

// how often we check for a new bundle, in ms
#define RECV_CHECK_TIME 100

void bpInit(char *src, char *dst, int md);
void bpQuit();
void bpSendMessage(void *m, int len);

#endif

