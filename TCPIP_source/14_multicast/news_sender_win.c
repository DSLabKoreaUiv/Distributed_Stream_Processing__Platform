/*
 * news_sender_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // For IP_MULTICAST_TTL Option.

#define TTL 64
#define BUFSIZE 30
void ErrorHandling(char *message);

int main(int argc, char **argv)
{
  WSADATA wsaData;

  SOCKET hSendSock;
  SOCKADDR_IN multiAddr;
  int multiTTL=TTL;
  int state;
  FILE *fp;
  char buf[BUFSIZE];


  if(argc!=3){
    printf("Usage : %s <Multicast IP> <port>\n", argv[0]);
    exit(1);
  }

  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
		ErrorHandling("WSAStartup() error!"); 
  
  hSendSock=socket(PF_INET, SOCK_DGRAM, 0); /*멀티캐스트를 위한 UDP소켓 생성 */
  if(hSendSock == INVALID_SOCKET)
    ErrorHandling("socket() error");

  memset(&multiAddr, 0, sizeof(multiAddr));
  multiAddr.sin_family=AF_INET;
  multiAddr.sin_addr.s_addr=inet_addr(argv[1]); /* 멀티캐스트 IP 설정 */
  multiAddr.sin_port=htons(atoi(argv[2]));		/* 멀티캐스트 port 설정 */

  state=setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*) &multiTTL, sizeof(multiTTL));
  if(state==SOCKET_ERROR)
    ErrorHandling("setsockopt() error");
  
  if((fp=fopen("News.txt", "r"))==NULL) /* news 파일 오픈 */
	  ErrorHandling("fopen() error");

  while(!feof(fp)){	/* news 방송 */
	  fgets(buf, BUFSIZE, fp);
	  sendto(hSendSock, buf, strlen(buf), 0, (SOCKADDR*)&multiAddr, sizeof(multiAddr));
	  Sleep(2000);
  }

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