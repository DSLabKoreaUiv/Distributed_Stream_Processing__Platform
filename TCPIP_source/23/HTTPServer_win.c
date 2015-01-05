/*
 * HTTPServer_win.c
 * Written by SW. YOON.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#define BUFSIZE 1024
#define SMALLBUF 100
DWORD WINAPI ClntConnect(void *arg);
char* ContentType(char* file);
void SendData(SOCKET sock, char* ct, char* fileName);
void SendErrorMSG(SOCKET sock);
void ErrorHandling(char *message);

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET hServSock;
  SOCKET hClntSock;

  HANDLE hThread;
  DWORD dwThreadID;

  SOCKADDR_IN servAddr;
  SOCKADDR_IN clntAddr;
  int clntAddrSize;

  if(argc!=2){
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");

  hServSock=socket(PF_INET, SOCK_STREAM, 0);   
  if(hServSock == INVALID_SOCKET)
    ErrorHandling("socket() error");
 
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family=AF_INET;
  servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servAddr.sin_port=htons(atoi(argv[1]));

  if(bind(hServSock, (SOCKADDR*) &servAddr, sizeof(servAddr))==SOCKET_ERROR)
    ErrorHandling("bind() error");

  if(listen(hServSock, 5)==SOCKET_ERROR)
    ErrorHandling("listen() error");

  /* ������ ���� �� ���� */
  while(1){
	  clntAddrSize=sizeof(clntAddr);
	  hClntSock=accept(hServSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
	  if(hClntSock==INVALID_SOCKET)
		  ErrorHandling("accept() error");

	  printf("�����û : %s:%d\n", inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port));

	  hThread = (HANDLE)_beginthreadex(NULL, 0, ClntConnect, (void*)hClntSock, 0, (unsigned *)&dwThreadID);
	  if(hThread == 0) {
		  puts("������ ���� ����");
		  exit(1);
	  }
  }

  WSACleanup();
  return 0;
}

DWORD WINAPI ClntConnect(void *arg)
{
  SOCKET hClntSock=(SOCKET)arg;
  char buf[BUFSIZE];
  char method[SMALLBUF];
  char ct[SMALLBUF];
  char fileName[SMALLBUF];
  
  recv(hClntSock, buf, BUFSIZE, 0);
  if(strstr(buf, "HTTP/")==NULL){ // HTTP�� ���� ��û���� Ȯ��
	  SendErrorMSG(hClntSock);
	  closesocket(hClntSock);
	  return 1;
  }

  strcpy(method, strtok(buf, " /"));
  if(strcmp(method, "GET")) // GET ��� ��û���� Ȯ�� 
	  SendErrorMSG(hClntSock);

  strcpy(fileName, strtok(NULL, " /")); // ��û �����̸� Ȯ��
  strcpy(ct, ContentType(fileName)); // Content-type Ȯ�� 

  SendData(hClntSock, ct, fileName); // ��  ��
  return 0;
}

void SendData(SOCKET sock, char* ct, char* fileName) 
{	
	char protocol[]="HTTP/1.0 200 OK\r\n";
	char server[]="Server:Best Http Server \r\n";
	char cntLen[]="Content-length:2048\r\n";
	char cntType[SMALLBUF];
	char buf[BUFSIZE];
	FILE* send_file;
	int len;

	sprintf(cntType, "Content-type:%s\r\n\r\n", ct);

	send_file = fopen(fileName, "r");
	if(send_file == NULL){
		SendErrorMSG(sock);
		return;
	}

	/* ��� ���� ���� */
	send(sock, protocol, strlen(protocol), 0);
	send(sock, server, strlen(server), 0);
	send(sock, cntLen, strlen(cntLen), 0); 
	send(sock, cntType, strlen(cntType), 0);

	/* ��û ������ ���� */
	while( len=fgets(buf, BUFSIZE, send_file ) != NULL) {
		send(sock, buf, strlen(buf), 0);
	}

	closesocket(sock); /* HTTP �������ݿ� ���ؼ� ���� �� ���� */
}

void SendErrorMSG(SOCKET sock) /* ���� �߻��� �޽��� ���� */
{	
	char protocol[]="HTTP/1.0 400 Bad Request\r\n";
	char server[]="Server:Best Http Server \r\n";
	char cntLen[]="Content-length:2048\r\n";
	char cntType[]="Content-type:text/html\r\n\r\n";
	char content[]="<html><head><title>NETWORK</title></head>"
	       "<body><font size=+5><br>���� �߻�! ��û ���ϸ� �� ��û ��� Ȯ��!"
		   "</font></body></html>";

	send(sock, protocol, strlen(protocol), 0);
	send(sock, server, strlen(server), 0);
	send(sock, cntLen, strlen(cntLen), 0); 
	send(sock, cntType, strlen(cntType), 0);
	send(sock, content, strlen(content), 0);

	closesocket(sock);
}

char* ContentType(char* file){ /* Content-Type ���� */
  char extension[SMALLBUF];
  char fileName[SMALLBUF];
  strcpy(fileName, file);
  strtok(fileName, ".");
  strcpy(extension, strtok(NULL, "."));
  if(!strcmp(extension, "html")||!strcmp(extension, "htm")) return "text/html";
  if(!strcmp(extension, "txt")||!strcmp(extension, "c")) return "text/plain";

  return "text/plain";
}


void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}