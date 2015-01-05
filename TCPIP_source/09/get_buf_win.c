/*
 * get_buf_win.c
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
	SOCKET hSocket;
	
	int sndBuf;
	int rcvBuf;
	int state;
	int len;
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	
	len=sizeof(sndBuf);
	state=getsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, &len);
	if(state==SOCKET_ERROR)
		ErrorHandling("getsockopt() error");
	
	len=sizeof(rcvBuf);
	state=getsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, &len);
	if(state==SOCKET_ERROR)
		ErrorHandling("getsockopt() error");
	
	printf("윈도우즈 입력 버퍼 크기 : %d \n", rcvBuf);
	printf("윈도우즈 출력 버퍼 크기 : %d \n", sndBuf);
	
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}