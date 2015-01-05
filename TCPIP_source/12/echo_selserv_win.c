/*
 * echo_selserv_win.c
 * Written by SW. YOON 
 */

#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 100
void ErrorHandling(char *message);

int main(int argc, char **argv)
{
  WSADATA wsaData;
  SOCKET hServSock;
  SOCKADDR_IN servAddr;

  int arrIndex;  
  int clntLen;
  SOCKET hClntSock;
  SOCKADDR_IN clntAddr;

  fd_set reads, temps;
  
  char message[BUFSIZE];
  int strLen;
  TIMEVAL timeout; //struct timeval timeout; 와 동일한 선언. 
  
  if(argc!=2){
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }
  
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
	  ErrorHandling("WSAStartup() error!");

  hServSock = socket(PF_INET, SOCK_STREAM, 0);
  if(hServSock==INVALID_SOCKET)
	  ErrorHandling("socket() error");

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(atoi(argv[1]));

  if(bind(hServSock, (SOCKADDR*) &servAddr, sizeof(servAddr))==SOCKET_ERROR)
	  ErrorHandling("bind() error");
  if(listen(hServSock, 5)==SOCKET_ERROR)
	  ErrorHandling("listen() error");
  
  FD_ZERO(&reads);
  FD_SET(hServSock, &reads);  

  while(1)
  {
    temps=reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0; 
    
    if(select(0, &temps, 0, 0, &timeout)==SOCKET_ERROR)
		ErrorHandling("select() error");

    for(arrIndex=0; arrIndex<reads.fd_count; arrIndex++) 
	{
		if(FD_ISSET(reads.fd_array[arrIndex], &temps))
		{
			if(temps.fd_array[arrIndex]==hServSock) //연결 요청일 경우.
			{
				clntLen = sizeof(clntAddr);
				hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntLen);
				FD_SET(hClntSock, &reads);				
				printf("클라이언트 연결 : 소켓 핸들 %d \n", hClntSock);
			}
			else //전송 된 데이터가 존재 하는 경우. 종료 요청 혹은 데이터 수신.
			{
				strLen = recv(temps.fd_array[arrIndex], message, BUFSIZE-1, 0);
				if(strLen ==0) //종료 요청의 경우.
				{
					FD_CLR(temps.fd_array[arrIndex], &reads);
					closesocket(temps.fd_array[arrIndex]);
					printf("클라이언트 종료 : 소켓 핸들 %d \n", temps.fd_array[arrIndex]);				
				}
				else //데이터 수신의 경우.
				{					
					send(temps.fd_array[arrIndex], message, strLen, 0);
				}	
			}
		} //if(FD_ISSET(reads.fd_array[arrIndex], &temps)) end
    } //for(arrIndex=0; arrIndex<reads.fd_count; arrIndex++)  end
  } //while(1) end

  WSACleanup();
  return 0;
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
