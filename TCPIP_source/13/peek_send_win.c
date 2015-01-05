/*
 * peek_send_win.c
 * Written by Yoon Sung Woo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void ErrorHandling(char *message);

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET sock;
  SOCKADDR_IN recvAddr;

  if(argc!=3){
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(0);
  }
  
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");

  sock=socket(PF_INET, SOCK_STREAM, 0);
  if(sock==INVALID_SOCKET)
    ErrorHandling("socket() error");

  memset(&recvAddr, 0, sizeof(recvAddr));
  recvAddr.sin_family=AF_INET;
  recvAddr.sin_addr.s_addr=inet_addr(argv[1]);
  recvAddr.sin_port=htons(atoi(argv[2]));
  
  if(connect(sock, (struct sockaddr*)&recvAddr, sizeof(recvAddr))==SOCKET_ERROR)
    ErrorHandling("connect() error!");

  send(sock, "123", 3, 0);

  closesocket(sock);
  WSACleanup();
  return 0;
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
