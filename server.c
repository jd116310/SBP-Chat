#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sbp_api.h"

#define SBP_BUFSZ (8192)
#define TIMESLICE (20000)

int interrupted = 0;

void hInterrupt(int sig)
{
	interrupted = 1;
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s <sink_eid>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	SBP_Init();
	SBP_Connection *conn = SBP_CreateConnection(argv[1]);
	if(conn == NULL)
	{
		SBP_FATAL("Unable to open connection. . .");
	}
	signal(SIGINT, hInterrupt);
	conn->blocking = SBP_POLL;
	while(!interrupted)
	{
		int res = conn->listen(conn);
		if(res >= 0)
		{
			char bundle_buffer[SBP_BUFSZ];
			int bundle_size;
			memset(bundle_buffer, 0, SBP_BUFSZ);
			bundle_size = SBP_GetDeliverySize(conn);
			SBP_GetDeliveryData(conn, bundle_buffer, bundle_size);
			SBP_SetDestination(conn, SBP_GetDeliverySource(conn));
			conn->send(conn, bundle_buffer, bundle_size);
			
			printf("Bundle from %s\n", SBP_GetDeliverySource(conn));
		}
		usleep(TIMESLICE);
	}
	SBP_DestroyConnection(conn);
	SBP_Shutdown();
	exit(EXIT_SUCCESS);
}

