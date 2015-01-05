/*
 * endian_conv_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void ErrorHandling(char *message);


int main(int argc, char **argv)
{
	WSADATA	wsaData;
	short hostOrdPort=0x1234;
	short netOrdPort;

	long hostOrdAdd=0x12345678;
	long netOrdAdd;

	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
		ErrorHandling("WSAStartup() error!");

	netOrdPort=htons(hostOrdPort);
	netOrdAdd=htonl(hostOrdAdd);

	printf(" Host ordered port :%x \n", hostOrdPort);
	printf(" Network ordered port : %x \n\n", netOrdPort);
 
	printf(" Host ordered Address : %x \n", hostOrdAdd);
	printf(" Network ordered Address : %x \n\n", netOrdAdd);

	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
