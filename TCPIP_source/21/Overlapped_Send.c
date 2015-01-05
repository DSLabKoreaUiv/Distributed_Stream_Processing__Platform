/*
 * Overlapped_Send.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(char *message);

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET hSocket;
  SOCKADDR_IN recvAddr;

  WSABUF dataBuf;
  char message[]="Network is Computer!";
  int sendBytes=0;

  WSAEVENT event;
  WSAOVERLAPPED overlapped;

  if(argc!=3){
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
		ErrorHandling("WSAStartup() error!"); 

  hSocket=WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  if(hSocket == INVALID_SOCKET)
    ErrorHandling("socket() error");

  memset(&recvAddr, 0, sizeof(recvAddr));
  recvAddr.sin_family=AF_INET;
  recvAddr.sin_addr.s_addr=inet_addr(argv[1]);
  recvAddr.sin_port=htons(atoi(argv[2]));

  if(connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr))==SOCKET_ERROR)
    ErrorHandling("connect() error!");
  
  event = WSACreateEvent();
  memset(&overlapped, 0, sizeof(overlapped));

  overlapped.hEvent=event;
  dataBuf.len=strlen(message);
  dataBuf.buf=message;

  if(WSASend(hSocket, &dataBuf, 1, &sendBytes, 0, &overlapped, NULL)==SOCKET_ERROR)
  {
	  if(WSAGetLastError() != WSA_IO_PENDING)
		  ErrorHandling("WSASend() error");
  }

  // 데이터 전송 진행 중에 다른 작업을 한다.
  puts("데이터 전송 중에 다른 일을 하고 있습니다");
  puts("데이터 전송은 Background로 진행 중에 있습니다.");

  WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE); //데이터 전송 끝났는지 확인.  
  WSAGetOverlappedResult(hSocket, &overlapped, &sendBytes, FALSE, NULL);
  printf("전송 된 바이트 수 : %d \n", sendBytes);
 
  closesocket(hSocket);
  WSACleanup();
  return 0;
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

