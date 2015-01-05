/*
 * make_socket_win.c
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
	SOCKET hTCPSock;		
	SOCKET hUDPSock;	

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
		ErrorHandling("WSAStartup() error!");
	
	/* 楷搬瘤氢 TCP 家南 积己 */
	hTCPSock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(hTCPSock==INVALID_SOCKET)
		ErrorHandling("socket() error");

	/* 厚 楷搬瘤氢 UDP 家南 积己 */
	hUDPSock=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(hUDPSock==INVALID_SOCKET)
		ErrorHandling("socket() error");

	printf("积己等 TCP 家南狼 勤甸 : %d \n", hTCPSock);
    printf("积己等 UDP 家南狼 勤甸 : %d \n", hUDPSock);
	
	closesocket(hTCPSock);	
	closesocket(hUDPSock);

	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
