/*
 * Routines_Receiver.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUFSIZE 1024
void CALLBACK CompletionRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(char *message);

WSABUF dataBuf;
char buf[BUFSIZE];
int recvBytes=0;

int main(int argc, char** argv)
{
	WSADATA wsaData;
	SOCKET hRecvSock;
	SOCKET hSendSock;	

	SOCKADDR_IN recvAddr;
	SOCKADDR_IN sendAddr;

	WSAOVERLAPPED overlapped;
	WSAEVENT event;

	int sendAddrSize;
	int index;	
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
	while(1)
	{
	   hSendSock=accept(hRecvSock, (SOCKADDR*)&sendAddr,&sendAddrSize);
	   if(hSendSock==INVALID_SOCKET)
		   ErrorHandling("accept() error");
	   
	   memset(&overlapped, 0, sizeof(overlapped));
	   dataBuf.len=BUFSIZE-1;
	   dataBuf.buf=buf;	   
	   event = WSACreateEvent(); /*Dummy event object */	     

	   while(1)
	   {
		   //데이터 수신 끝났는지 확인. 다섯번째 전달 인자가 TRUE인지 의미 하는 바가 무엇인지 반드시 설명.
		   flags=0;
		   memset(&overlapped, 0, sizeof(overlapped));
		   dataBuf.len=BUFSIZE-1;
		   dataBuf.buf=buf;	

		   if(WSARecv(hSendSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, CompletionRoutine)==SOCKET_ERROR)
		   {
			   if(WSAGetLastError() != WSA_IO_PENDING)
				   ErrorHandling("WSARecv() error");
		   }
	
		   index=WSAWaitForMultipleEvents(1, &event, FALSE, WSA_INFINITE, TRUE); 
		   if(index==WAIT_IO_COMPLETION) //중첩 된 입 출력이 완료 되었다면,
		   {
			   if(recvBytes==0){ //EOF 입력시 
				   closesocket(hSendSock);
				   break;
			   }
		   }
		   else // 오류 발생 시
		   {
			   closesocket(hSendSock);
			   break;
		   }
	   } // while(1) end
	} // while(1) end

	WSACleanup();
	return 0;
}

void CALLBACK CompletionRoutine(DWORD error, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	if(error!=0) //에러 발생시
		ErrorHandling("CompletionRoutine error");	
	recvBytes=szRecvBytes;
	buf[szRecvBytes]=0;
	printf("수신한 메시지 : %s \n", buf);
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
