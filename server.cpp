#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "sbp_api.h"
#include "list.h"

#define SBP_BUFSZ 8192

int interrupted = 0;

void hInterrupt(int sig)
{
	interrupted = 1;
}

void getTime(char *buff)
{
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime (&rawtime);
	strftime(buff, TIME_STR_LEN, "%X", timeinfo);
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
	conn->blocking = SBP_BLOCKING;
	while(!interrupted)
	{
		int res = conn->listen(conn);
		if(res >= 0)
		{
			char now[TIME_STR_LEN];
			char bundle_buffer[SBP_BUFSZ];
			int bundle_size;
			item *i;
			
			memset(bundle_buffer, 0, SBP_BUFSZ);
			bundle_size = SBP_GetDeliverySize(conn);
			SBP_GetDeliveryData(conn, bundle_buffer, bundle_size);
			
			if(bundle_size == 2)
			{
				printf("Got keep alive from %s\n", SBP_GetDeliverySource(conn));
			}
			else
			{
				SBP_SetDestination(conn, SBP_GetDeliverySource(conn));
				conn->send(conn, bundle_buffer, bundle_size);
			
				getTime(now);
				i = (item *) bundle_buffer;
			
				printf("Bundle from %s\n\tTimestamp: %s\n\tNow:       %s\n", SBP_GetDeliverySource(conn), i->time, now);
			}
		}
	}
	SBP_DestroyConnection(conn);
	SBP_Shutdown();
	exit(EXIT_SUCCESS);
}

