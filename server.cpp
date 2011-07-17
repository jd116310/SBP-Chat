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

#define SBP_BUFSZ 8192


using namespace std;


map<string, time_t> clients;
int interrupted = 0;


void hInterrupt(int sig)
{
	interrupted = 1;
}

void getTime(char *buff, time_t rawtime)
{
	struct tm * timeinfo;
	timeinfo = localtime (&rawtime);

	strftime(buff, TIME_STR_LEN, "%X", timeinfo);
}

int main(int argc, char *argv[])
{
	int max_delay;

	// Make sure we have enough arguments
	// This could be made alot more robust
	if(argc < 3)
	{
		fprintf(stderr, "Usage: %s <sink_eid> <max_delay>\n", argv[0]);
		fprintf(stderr, "\tsink_eid - the eid the server listens on\n");
		fprintf(stderr, "\tmax_delay - the maximum delay in seconds between client and server\n");
		return 1;
	}
	
	// Get the maximum delay
	max_delay = atoi(argv[2]);
	
	// Initialize the Bundle Protocol
	SBP_Init();
	SBP_Connection *conn = SBP_CreateConnection(argv[1]);

	if(conn == NULL)
	{
		SBP_FATAL("Unable to open connection. . .");
	}
	conn->blocking = SBP_BLOCKING;
	
	signal(SIGINT, hInterrupt);
	
	while(!interrupted)
	{
		int res = conn->listen(conn);
		if(res >= 0)
		{
			char nowBuffer[TIME_STR_LEN];
			char thenBuffer[TIME_STR_LEN];
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
				
				i = (item *) bundle_buffer;
				
				getTime(nowBuffer, cur_time);
				getTime(thenBuffer, i->timestamp);
				printf("Bundle from %s (%s)\n\tTimestamp: %s\n\tNow:       %s\n", source.c_str(), i->nick, thenBuffer, nowBuffer);
				
				// send to all clients connected. If the last time of contact
				// is to long, remove from list
				for(it = clients.begin(); it != clients.end(); /*blank*/)
				{
					if((cur_time - it->second) > (max_delay * 2))
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
	return 0;
}

