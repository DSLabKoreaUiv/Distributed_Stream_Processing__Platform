/*
 * chat_client_win.c
 * Written by SW. YOON
 */
#pragma comment (lib,"ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#define BUFSIZE 100
#define NAMESIZE 20

DWORD WINAPI SendMSG(void *arg);
DWORD WINAPI RecvMSG(void *arg);
void ErrorHandling(char *message);

char name[NAMESIZE]="[Default]";
char message[BUFSIZE];

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET sock;
  SOCKADDR_IN servAddr;
  
  HANDLE hThread1, hThread2;
  DWORD dwThreadID1, dwThreadID2;

   if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");
  //sprintf(name, "[%s]", argv[3]);

  sock=socket(PF_INET, SOCK_STREAM, 0);
  if(sock == INVALID_SOCKET)
    ErrorHandling("socket() error");

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family=AF_INET;
  servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
  servAddr.sin_port=htons(5150);
  
  if(connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
	  ErrorHandling("connect() error");
  
  hThread1 = (HANDLE)_beginthreadex(NULL, 0, SendMSG, (void*)sock, 0, (unsigned *)&dwThreadID1); 
  hThread2 = (HANDLE)_beginthreadex(NULL, 0, RecvMSG, (void*)sock, 0, (unsigned *)&dwThreadID2);
  if(hThread1==0 || hThread2==0) {
	  ErrorHandling("쓰레드 생성 오류");
  }
  
  WaitForSingleObject(hThread1, INFINITE);
  WaitForSingleObject(hThread2, INFINITE);
 
  closesocket(sock);
  return 0;
}

DWORD WINAPI SendMSG(void *arg) // 메시지 전송 쓰레드 실행 함수
{
   SOCKET sock = (SOCKET)arg;
   char nameMessage[NAMESIZE+BUFSIZE];
   while(1) {
      fgets(message, BUFSIZE, stdin);
      sprintf(nameMessage,"%s %s", name, message);
      if(!strcmp(message,"q\n")) {  // 'q' 입력시 종료
         closesocket(sock);
         exit(0);		 
      }
      send(sock, nameMessage, strlen(nameMessage), 0);
   }
}

DWORD WINAPI RecvMSG(void *arg) /* 메시지 수신 쓰레드 실행 함수 */
{
  SOCKET sock = (SOCKET)arg;
  char nameMessage[NAMESIZE+BUFSIZE];
  int strLen;
  while(1){
    strLen = recv(sock, nameMessage, NAMESIZE+BUFSIZE-1, 0);
	if(strLen==-1) return 1;
	
    nameMessage[strLen]=0;
    fputs(nameMessage, stdout);
  }
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
