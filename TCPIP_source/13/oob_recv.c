/*
 * oob_recv.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFSIZE 30
void error_handling(char *message);
void urg_handler(int signo);

int recv_sock;
int send_sock;

int main(int argc, char **argv)
{
  struct sockaddr_in recv_addr;
  struct sockaddr_in send_addr;
  int send_addr_size, str_len, state;
  struct sigaction act;         
  char buf[BUFSIZE];

  if(argc!=2){
    printf("Usage : %s <port>\n", argv[0]);  
    exit(0);
  }
  
  act.sa_handler=urg_handler;    
  sigemptyset(&act.sa_mask);    
  act.sa_flags=0; 

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

  fcntl(send_sock, F_SETOWN, getpid()); 
  state=sigaction(SIGURG, &act, 0);
  if(state != 0)
    error_handling("sigaction() error ");

  while( (str_len=recv(send_sock, buf, sizeof(buf), 0)) != 0) {
    if(str_len==-1)
      continue;
    buf[str_len]=0;
    puts(buf);
  }
  close(send_sock);
  return 0; 
}

void urg_handler(int signo)
{
  int str_len;
  char buf[BUFSIZE];
  str_len=recv(send_sock, buf, sizeof(buf)-1, MSG_OOB);
  buf[str_len]=0;
  printf("긴급 메시지 전송 : %s \n", buf);
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
