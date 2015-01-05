/*
 * chat_server_win.c
 * Written by SW. YOON
 */
#pragma comment (lib,"ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#define BUFSIZE 100

DWORD WINAPI ClientConn(void *arg);
void SendMSG(char* message, int len);
void ErrorHandling(char *message);

int clntNumber=0;
SOCKET clntSocks[10];
HANDLE hMutex;

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET servSock;
  SOCKET clntSock;

  SOCKADDR_IN servAddr;
  SOCKADDR_IN clntAddr;
  int clntAddrSize;

  HANDLE hThread;
  DWORD dwThreadID;

  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");

  hMutex=CreateMutex(NULL, FALSE, NULL);
  if(hMutex==NULL){
	  ErrorHandling("CreateMutex() error");
  }

  servSock=socket(PF_INET, SOCK_STREAM, 0);   
  if(servSock == INVALID_SOCKET)
    ErrorHandling("socket() error");
 
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family=AF_INET;
  servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servAddr.sin_port=htons(5150);

  if(bind(servSock, (SOCKADDR*) &servAddr, sizeof(servAddr))==SOCKET_ERROR)
    ErrorHandling("bind() error");

  if(listen(servSock, 5)==SOCKET_ERROR)
    ErrorHandling("listen() error");

  while(1){
	  clntAddrSize=sizeof(clntAddr);
	  clntSock=accept(servSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
	  if(clntSock==INVALID_SOCKET)
		  ErrorHandling("accept() error");

	  WaitForSingleObject(hMutex, INFINITE);
	  clntSocks[clntNumber++]=clntSock;
	  ReleaseMutex(hMutex);
	  printf("from  [CTL: JOIN REQUEST] - %s : %s\n", inet_ntoa(clntAddr.sin_addr), clntAddr.sin_port);

	  hThread = (HANDLE)_beginthreadex(NULL, 0, ClientConn, (void*)clntSock, 0, (unsigned *)&dwThreadID);
	  if(hThread == 0) {
		  ErrorHandling("쓰레드 생성 오류");
	  }
  }

  WSACleanup();
  return 0;
}

DWORD WINAPI ClientConn(void *arg)
{
  SOCKET clntSock=(SOCKET)arg;
  int strLen=0;
  char message[BUFSIZE];
  int i;

  while( (strLen=recv(clntSock, message, BUFSIZE, 0)) != 0)
    SendMSG(message, strLen);

  WaitForSingleObject(hMutex, INFINITE);
  for(i=0; i<clntNumber; i++){   // 클라이언트 연결 종료시
    if(clntSock == clntSocks[i]){
      for( ; i<clntNumber-1; i++)
		  clntSocks[i]=clntSocks[i+1];
      break;
    }
  }
  clntNumber--;
  ReleaseMutex(hMutex);

  closesocket(clntSock);
  return 0;
}

void SendMSG(char* message, int len)
{
	int i;
	WaitForSingleObject(hMutex, INFINITE); 
	for(i=0; i<clntNumber; i++)
		send(clntSocks[i], message, len, 0);
	ReleaseMutex(hMutex);	
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
