/*
 * inet_addr_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void ErerorHandling(char *message);

int main(int argc, char **argv)
{
	WSADATA	wsaData;

	char * addr1 = "1.2.3.4";
	char * addr2 = "1.2.3.256";
	unsigned long convAddr;
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
		ErerorHandling("WSAStartup() error!");

	convAddr = inet_addr(addr1);
	if(convAddr == INADDR_NONE)
		printf("Error Occur : %d \n", convAddr);
	else
		printf("Unsigned long addr(network ordered) : %x \n", convAddr);

	convAddr = inet_addr(addr2);
	if(convAddr == INADDR_NONE)
		printf("Error Occured : %d \n\n", convAddr);
	else
		printf("Unsigned long addr(network ordered) : %x \n\n", convAddr);

  	WSACleanup();
	return 0;
}

void ErerorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}