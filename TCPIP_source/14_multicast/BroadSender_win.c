/*
 * BroadSender_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 30
#define TRUE 1
#define FALSE 0
void ErrorHandling(char *message);

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET hSendSock;
  SOCKADDR_IN broadAddr;
  int state;
  FILE *fp;
  char buf[BUFSIZE];
  int so_broadcast=TRUE;

  if(argc!=3){
    printf("Usage : %s <Boradcast IP> <port>\n", argv[0]);
    exit(1);
  }

  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!"); 
  
  hSendSock=socket(PF_INET, SOCK_DGRAM, 0); /*브로드캐스트를 위해 UDP소켓 생성 */
  if(hSendSock == INVALID_SOCKET)
    ErrorHandling("socket() error");
 
  memset(&broadAddr, 0, sizeof(broadAddr));
  broadAddr.sin_family=AF_INET;
  broadAddr.sin_addr.s_addr=inet_addr(argv[1]);
  broadAddr.sin_port=htons(atoi(argv[2]));
  
  state=setsockopt(hSendSock, SOL_SOCKET, SO_BROADCAST, (char*)&so_broadcast, sizeof(so_broadcast));
  if(state==SOCKET_ERROR)
    ErrorHandling("setsockopt() error");
  
  if((fp=fopen("News.txt", "r"))==NULL) /* news 파일 오픈 */
	  ErrorHandling("fopen() error");

  while(!feof(fp)){	/* news 방송 */
	  fgets(buf, BUFSIZE, fp);
	  sendto(hSendSock, buf, strlen(buf), 0, (SOCKADDR*)&broadAddr, sizeof(broadAddr));
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
