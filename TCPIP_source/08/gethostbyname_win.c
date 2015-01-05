/*
 * gethostbyname_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 30
void ErrorHandling(char *message);

int main(int argc, char **argv)
{
	WSADATA  wsaData;
	
	int i;
	struct hostent *host; //HOSTENT* host
	
	if(argc!=2){
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	
	host=gethostbyname(argv[1]);
	if(!host)
		ErrorHandling("gethost... error");
	
	printf("Officially name : %s \n\n", host->h_name);
	
	puts("Aliases-----------");
	for(i=0;host->h_aliases[i]; i++){
		puts(host->h_aliases[i]);
	}
	
	printf("Address Type : %s \n", host->h_addrtype==AF_INET? "AF_INET":"AF_INET6");
	puts("IP Address--------");
	for(i=0; host->h_addr_list[i]; i++){
		puts( inet_ntoa( *(IN_ADDR*)host->h_addr_list[i] ));
	}
	
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}