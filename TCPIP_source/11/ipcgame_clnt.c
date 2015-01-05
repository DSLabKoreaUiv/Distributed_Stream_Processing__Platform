/*
 * ipcgame_clnt.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 100
void error_handling(char *message);

int main(int argc, char **argv)
{
  int sock;
  char message[BUFSIZE];
  int str_len;

  struct sockaddr_in serv_addr;

  if(argc!=3){
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  sock=socket(PF_INET, SOCK_STREAM, 0);   

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
  serv_addr.sin_port=htons(atoi(argv[2]));

  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
    error_handling("connect() error!");

  /* 가위 바위 보 게임 시작 */
  str_len=read(sock, message, BUFSIZE-1);
  message[str_len]=0;
  fputs(message, stdout);
  fflush(stdout);

  str_len = read(0, message, BUFSIZE);	 /* 클라이언트의 선택 입력 */
  write(sock, message, str_len);	     /* 클라이언트의 선택 전송 */

  str_len=read(sock, message,BUFSIZE-1); /* 승! 패! 결과 수신 */
  message[str_len]=0;
  puts(message);
  
  close(sock);
  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}