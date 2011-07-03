#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sbp_api.h>
#include "list.h"

#define SBP_BUFSZ (8192)

SBP_Connection *conn;

void bp_recv()
{
	int res = conn->listen(conn);
	if(res >= 0)
	{
		char bundle_buffer[SBP_BUFSZ];
		int bundle_size;
		memset(bundle_buffer, 0, SBP_BUFSZ);
		bundle_size = SBP_GetDeliverySize(conn);
		SBP_GetDeliveryData(conn, bundle_buffer, bundle_size);
		
		recvMessage(bundle_buffer);
	}
}

void check(int sig)
{
	bp_recv();
}

void bp_init(SBP_Connection *c)
{
	struct sigaction sigalrm_action;
	struct itimerval timer;
	
	timer.it_interval.tv_sec = 0;	//Deal only in usec
	timer.it_interval.tv_usec = 100000;
	timer.it_value.tv_sec = 0;	//Deal only in usec
	timer.it_value.tv_usec = 100000;	

	sigalrm_action.sa_handler = check;	
	sigemptyset(&sigalrm_action.sa_mask);
	sigalrm_action.sa_flags = 0;
	
	sigaction(SIGALRM, &sigalrm_action, 0);					
    	
	if(setitimer(ITIMER_REAL, &timer,NULL) != 0)
	{
		printf("Error starting timer");
		exit(1);
	}
	
	conn = c;
}

void bp_send()
{
		//SBP_SetDestination(conn, SBP_GetDeliverySource(conn));
		//conn->send(conn, bundle_buffer, bundle_size);
}


