#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sbp_api.h>
#include "list.h"
#include "bp.h"

#define SBP_BUFSZ (8192)

// The number of times we check for a recieved bundle
// until we send a keep alive (this does away with any threading needed)
#define CHECKS_BEFORE_KEEPALIVE (1000 * KEEP_ALIVE_TIME / RECV_CHECK_TIME)

SBP_Connection *conn;

void bpRecv()
{
	int res = conn->listen(conn);
	if(res >= 0)
	{
		char bundle_buffer[SBP_BUFSZ];
		int bundle_size;
		memset(bundle_buffer, 0, SBP_BUFSZ);
		bundle_size = SBP_GetDeliverySize(conn);
		SBP_GetDeliveryData(conn, bundle_buffer, bundle_size);
		
		recvMessage((void*)bundle_buffer);
	}
}

void bpSendKeepAlive()
{
	char keepalive[] = "KA";
	conn->send(conn, keepalive, strlen(keepalive));
}

void check(int sig)
{
	static int count = 0;
	bpRecv();
	
	if(++count >= CHECKS_BEFORE_KEEPALIVE)
	{
		bpSendKeepAlive();
		count = 0;
	}
}

void bpInit(char *src, char *dst)
{
	SBP_Init();
	conn = SBP_CreateConnection(src);
	if(conn == NULL)
	{
		SBP_FATAL("Unable to open connection. . .");
	}
	
	conn->blocking = SBP_POLL;
	SBP_SetDestination(conn, dst);

	// Set up timer for receiving 

	struct sigaction sigalrm_action;
	struct itimerval timer;
	
	timer.it_interval.tv_sec = 0;	//Deal only in usec
	timer.it_interval.tv_usec = RECV_CHECK_TIME * 1000;
	timer.it_value.tv_sec = 0;		//Deal only in usec
	timer.it_value.tv_usec = RECV_CHECK_TIME * 1000;	

	sigalrm_action.sa_handler = check;	
	sigemptyset(&sigalrm_action.sa_mask);
	sigalrm_action.sa_flags = 0;
	
	sigaction(SIGALRM, &sigalrm_action, 0);					
    	
	if(setitimer(ITIMER_REAL, &timer,NULL) != 0)
	{
		printf("Error starting timer");
		exit(1);
	}
	
	// Send a keep alive to let the server know we are there
	bpSendKeepAlive();
}

void bpQuit()
{
    SBP_DestroyConnection(conn);
	SBP_Shutdown();
}

void bpSendMessage(void *m, int len)
{
	conn->send(conn, m, len);
}


