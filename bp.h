#ifndef BP_H
#define BP_H

// how often we check for a new bundle, in ms
#define RECV_CHECK_TIME 100
// how often we send a keep alive, in seconds
#define KEEP_ALIVE_TIME 10

void bpInit(char *src, char *dst);
void bpQuit();
void bpSendMessage(void *m, int len);

#endif

