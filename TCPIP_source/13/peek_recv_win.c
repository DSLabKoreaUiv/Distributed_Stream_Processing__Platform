/*
 * peek_recv_win.c
 * Written by Yoon Sung Woo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 30
void ErrorHandling(char *message);

int main(int argc, char **argv)
{
	WSADATA	wsaData;
	SOCKET hRecvSock;
	SOCKET hSendSock;
	SOCKADDR_IN recvAddr;
	SOCKADDR_IN sendAddr;
	int sendAddrSize, strLen;
	char buf[BUFSIZE];

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);  
		exit(0);
	}

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
		ErrorHandling("WSAStartup() error!");
	
	hRecvSock=socket(PF_INET, SOCK_STREAM, 0);
	if(hRecvSock==INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family=AF_INET;
	recvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	recvAddr.sin_port=htons(atoi(argv[1]));
	
	if(bind(hRecvSock, (struct sockaddr*)&recvAddr, sizeof(recvAddr))==SOCKET_ERROR)
		ErrorHandling("bind() error");

	if(listen(hRecvSock, 5)==SOCKET_ERROR)
		ErrorHandling("listen() error");
	
	sendAddrSize=sizeof(sendAddr);
	hSendSock=accept(hRecvSock, (SOCKADDR*)&sendAddr, &sendAddrSize);
	Sleep(1);
	strLen=recv(hSendSock, buf, sizeof(buf)-1, MSG_PEEK); //MSG_DONTWAIT음따
	buf[strLen]=0;
	printf("총 %d 바이트 존재합니다 : %s\n", strLen, buf);
	
	strLen=recv(hSendSock, buf, sizeof(buf)-1, 0);
	buf[strLen]=0;
	printf("읽어 온 결과 입니다. : %s\n", buf);
	
	closesocket(hSendSock);
	WSACleanup();
	return 0; 
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}