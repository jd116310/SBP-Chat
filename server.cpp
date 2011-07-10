#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <map>
#include "sbp_api.h"
#include "list.h"
#include "bp.h"

#define SBP_BUFSZ 8192


using namespace std;


map<string, time_t> clients;
int interrupted = 0;


void hInterrupt(int sig)
{
	interrupted = 1;
}

void getTime(char *buff)
{
	time_t rawtime;
	struct tm *timeinfo;

	rawtime = time(NULL);
	timeinfo = localtime(&rawtime);
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
			string source(SBP_GetDeliverySource(conn));
			
			memset(bundle_buffer, 0, SBP_BUFSZ);
			bundle_size = SBP_GetDeliverySize(conn);
			SBP_GetDeliveryData(conn, bundle_buffer, bundle_size);
			
			// Update the time of the clients last contact
			clients[source] = time(NULL);
			
			if(bundle_size == 2)
			{
				printf("Got keep alive from %s\n", source.c_str());
			}
			else
			{
				item *i;
				time_t cur_time = time(NULL);
				map<string, time_t>::iterator it;
				
				getTime(now);
				i = (item *) bundle_buffer;
				printf("Bundle from %s\n\tTimestamp: %s\n\tNow:       %s\n", source.c_str(), i->time, now);
				
				// send to all clients connected. If the last time of contact
				// is to long, remove from list
				for(it = clients.begin(); it != clients.end(); /*blank*/)
				{
					if((cur_time - it->second) > (KEEP_ALIVE_TIME * 2))
					{
						map<string, time_t>::iterator erase_element = it++;
						
						printf("Removing %s\n", erase_element->first.c_str());
						
						clients.erase(erase_element);
					}
					else
					{
						char dest_name[128];
						
						// copy the dest into a non constant char array
						strcpy(dest_name, &(it->first.c_str()[0]));
						
						// set destination and send
						SBP_SetDestination(conn, dest_name);
						conn->send(conn, bundle_buffer, bundle_size);	
						
						printf("Sending to %s\n", it->first.c_str());
						it++;					
					}
				}
			}
		}
	}
	
	SBP_DestroyConnection(conn);
	SBP_Shutdown();
	exit(EXIT_SUCCESS);
}

