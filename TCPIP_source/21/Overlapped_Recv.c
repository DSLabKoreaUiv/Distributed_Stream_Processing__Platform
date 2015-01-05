/*
 * Overlapped_Recv.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUFSIZE 1024
void ErrorHandling(char *message);

int main(int argc, char** argv)
{
	WSADATA wsaData;
	SOCKET hRecvSock;
	SOCKET hSendSock;

	SOCKADDR_IN recvAddr;
	SOCKADDR_IN sendAddr;
	int sendAddrSize;

	WSABUF dataBuf;
	WSAEVENT event;
	WSAOVERLAPPED overlapped;

	char buf[BUFSIZE];
	int recvBytes=0;
	int flags=0;		

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
		ErrorHandling("WSAStartup() error!"); 

	hRecvSock=WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(hRecvSock == INVALID_SOCKET)
		ErrorHandling("socket() error");
 
	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family=AF_INET;
	recvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	recvAddr.sin_port=htons(atoi(argv[1]));

	if(bind(hRecvSock, (SOCKADDR*) &recvAddr, sizeof(recvAddr))==SOCKET_ERROR)
		ErrorHandling("bind() error");

	if(listen(hRecvSock, 5)==SOCKET_ERROR)
		ErrorHandling("listen() error");

	sendAddrSize=sizeof(sendAddr);    
	hSendSock=accept(hRecvSock, (SOCKADDR*)&sendAddr,&sendAddrSize);
	if(hSendSock==INVALID_SOCKET)
		ErrorHandling("accept() error");

	event = WSACreateEvent();
	memset(&overlapped, 0, sizeof(overlapped));

	overlapped.hEvent=event;
	dataBuf.len=BUFSIZE-1;
	dataBuf.buf=buf;

	if(WSARecv(hSendSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, NULL)==SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
			ErrorHandling("WSARecv() error");
	}

	// 데이터 수신 진행 중에 다른 작업을 한다.
	puts("데이터 수신 중에 다른 일을 하고 있습니다");
	puts("데이터 수신은 Background로 진행 중에 있습니다.");

	WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE);
	WSAGetOverlappedResult(hSendSock, &overlapped, &recvBytes, FALSE, NULL);
	buf[recvBytes]=0;
	printf("수신한 메시지 : %s \n", buf);

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
