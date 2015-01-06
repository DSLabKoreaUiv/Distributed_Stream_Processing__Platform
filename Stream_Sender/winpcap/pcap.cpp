#include "Pcap.h"


void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

char send_string[1024];
int id=0;

int main(int argc, char* argv[])
{
pcap_if_t *alldevs;
pcap_if_t *d;

CStreamCommuniCation* Stream_Sender;
Stream_Sender=new CStreamCommuniCation(argv[1],atoi(argv[2]));



int inum;
int i=0;
pcap_t *adhandle;
char errbuf[PCAP_ERRBUF_SIZE];


    
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }
    
    /* Print the list */
    for(d=alldevs; d; d=d->next)
    {
        printf("%d. %s", ++i, d->name);
        if (d->description)
            printf(" (%s)\n", d->description);
        else
            printf(" (No description available)\n");
    }
    
    if(i==0)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        return -1;
    }
    
    printf("Enter the interface number (1-%d):",i);
    scanf_s("%d", &inum);
    
    if(inum < 1 || inum > i)
    {
        printf("\nInterface number out of range.\n");
        /* Free the device list */
        pcap_freealldevs(alldevs);
        return -1;
    }
  
    for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
    

    if ( (adhandle= pcap_open(d->name,         
                              65536,        
                                              
                              PCAP_OPENFLAG_PROMISCUOUS,   
                              1000,         
                              NULL,          
                              errbuf         
                              ) ) == NULL)
    {
        fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
        /* Free the device list */
        pcap_freealldevs(alldevs);
        return -1;
    }
    
    printf("\nlistening on %s...\n", d->description);
    
    /* At this point, we don't need any more the device list. Free it */
    pcap_freealldevs(alldevs);
    
    /* start the capture */
    pcap_loop(adhandle, 0, packet_handler,(u_char*)Stream_Sender);

	delete Stream_Sender;
    
    return 0;
}



void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{


    struct tm ltime;
    char timestr[16];
    time_t local_tv_sec;
	

	CStreamCommuniCation* comm=(CStreamCommuniCation*)param;
	
	id++;
	/*
     * unused variables
     */
   // (VOID)(param);
   // (VOID)(pkt_data);

	
    local_tv_sec = header->ts.tv_sec;
    localtime_s(&ltime, &local_tv_sec);
    strftime(timestr, sizeof timestr, "%H:%M:%S", &ltime);
    printf("id: %d %s,%.6d len:%d\n",id, timestr, header->ts.tv_usec, header->len);
	sprintf(send_string,"ID:%d %s,%.6d len:%d\n",id,timestr, header->ts.tv_usec, header->len);
	comm->Send(send_string);
	
	    
}

