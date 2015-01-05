/*
 * oob_send_win.c
 * Written by SW. YOON.
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUFSIZE 30
void ErrorHandling(char *message);

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET hSocket;
  SOCKADDR_IN recvAddr;

  if(argc!=3){
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(0);
  }

  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");

  hSocket=socket(PF_INET, SOCK_STREAM, 0);
  if(hSocket==INVALID_SOCKET )
	  ErrorHandling("socket() error");

  memset(&recvAddr, 0, sizeof(recvAddr));
  recvAddr.sin_family=AF_INET;
  recvAddr.sin_addr.s_addr=inet_addr(argv[1]);
  recvAddr.sin_port=htons(atoi(argv[2]));
  
  if(connect(hSocket, (struct sockaddr*)&recvAddr, sizeof(recvAddr))==SOCKET_ERROR)
	  ErrorHandling("connect() error!");

  send(hSocket, "123", 3, 0);
  send(hSocket, "4", 1, MSG_OOB);
  send(hSocket, "567", 3, 0);
  send(hSocket, "890", 3, MSG_OOB);
 
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
