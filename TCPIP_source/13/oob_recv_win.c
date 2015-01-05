/*
 * oob_recv_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUFSIZE 30
void ErrorHandling(char *message);

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET hRecvSock, hSendSock;

  SOCKADDR_IN recvAddr;
  SOCKADDR_IN sendAddr;  
  int sendAddrSize, strLen;
  char buf[BUFSIZE];
  int result, fd_max;

  fd_set read, except, readCopy, exceptCopy;
  struct timeval timeout;

  if(argc!=2){
    printf("Usage : %s <port>\n", argv[0]);  
    exit(0);
  }

  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");
 
  hRecvSock=socket(PF_INET, SOCK_STREAM, 0);
  if(hRecvSock==INVALID_SOCKET)
    ErrorHandling("socket() error");

  memset(&recvAddr, 0, sizeof(recvAddr));
  recvAddr.sin_family=AF_INET;
  recvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
  recvAddr.sin_port=htons(atoi(argv[1]));

  if(bind(hRecvSock, (SOCKADDR*)&recvAddr, sizeof(recvAddr))==SOCKET_ERROR)
	  ErrorHandling("bind() error");
  if(listen(hRecvSock, 5)==SOCKET_ERROR)
	  ErrorHandling("bind() error");
 

  sendAddrSize=sizeof(sendAddr);
  hSendSock=accept(hRecvSock, (SOCKADDR*)&sendAddr, &sendAddrSize);
  FD_ZERO(&read);
  FD_ZERO(&except);
  FD_SET(hSendSock, &read);
  FD_SET(hSendSock, &except);
  fd_max=hSendSock;

  while(1)
  {  
    readCopy=read;
	exceptCopy=except;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0; 
    
    result = select(fd_max+1, &readCopy, 0, &exceptCopy, &timeout);

	if(result>0)
	{
		if(FD_ISSET(hSendSock, &exceptCopy)){
			strLen=recv(hSendSock, buf, BUFSIZE-1, MSG_OOB);
			buf[strLen]=0;
			printf("긴급 메시지 전송 : %s \n", buf);
		}	

		if(FD_ISSET(hSendSock, &readCopy)){
			strLen = recv(hSendSock, buf, BUFSIZE-1, 0);
			if(strLen ==0) break;				
			else {	   
				buf[strLen]=0;
				puts(buf); 
			}	
		}				
	} // if(result>0) end.
  } //while(1) end.
  
  closesocket(hSendSock);
  WSACleanup( );
  return 0; 
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
