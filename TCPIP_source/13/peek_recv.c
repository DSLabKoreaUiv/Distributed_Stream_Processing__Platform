/*
 * peek_recv.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 30
void error_handling(char *message);

int main(int argc, char **argv)
{
  int recv_sock;   
  int send_sock;  

  struct sockaddr_in recv_addr;
  struct sockaddr_in send_addr;
  int send_addr_size, str_len, state;

  char buf[BUFSIZE];

  if(argc!=2){
    printf("Usage : %s <port>\n", argv[0]);  
    exit(0);
  }

  recv_sock=socket(PF_INET, SOCK_STREAM, 0);
  if(recv_sock==-1)
    error_handling("socket() error");

  memset(&recv_addr, 0, sizeof(recv_addr));
  recv_addr.sin_family=AF_INET;
  recv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  recv_addr.sin_port=htons(atoi(argv[1]));

  
  if(bind(recv_sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr))==-1)
    error_handling("bind() error");
  listen(recv_sock, 5);

  send_addr_size=sizeof(send_addr);
  send_sock=accept(recv_sock, (struct sockaddr*)&send_addr, &send_addr_size);

  sleep(1);
  str_len=recv(send_sock, buf, sizeof(buf)-1, MSG_PEEK|MSG_DONTWAIT);
  buf[str_len]=0;
  printf("총 %d 바이트 존재합니다 : %s\n", str_len, buf);
 
  str_len=recv(send_sock, buf, sizeof(buf)-1, 0);
  buf[str_len]=0;
  printf("읽어 온 결과 입니다. : %s\n", buf);

  close(send_sock);
  return 0; 
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}