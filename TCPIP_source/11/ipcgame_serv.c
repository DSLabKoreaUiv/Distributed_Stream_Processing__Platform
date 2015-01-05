/*
 * ipcgame_serv.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define BUFSIZE 100
void error_handling(char *message);
void z_handler(int sig); 
int who_win(int a, int b);

int main(int argc, char **argv)
{
  int fd1[2], fd2[2];

  char buffer[BUFSIZE];
  char intro[]="입력 하세요(가위:0, 바위:1, 보:2) : "; /*바이트 비교시 48, 49, 50.*/
  char win[] = "축하 합니다 당신이 이겼습니다. \n";
  char lose[] = "안타깝게도 졌네요. \n";
  char no_winner[] = "비겼네요. 승자가 없습니다. \n";
  
  int serv_sock;
  int clnt_sock;
  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  struct sigaction act;
  int str_len, state, addr_size;
  pid_t pid;

  if(argc!=2){
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  if(pipe(fd1)<0 || pipe(fd2)<0)	/* 프로세스 양 방향 통신 위한 파이프 생성 */
    error_handling("pipe() error");

  act.sa_handler=z_handler; /* 시그널 핸들링을 위한 핸들러 설정 */
  sigemptyset(&act.sa_mask);
  act.sa_flags=0;

  state=sigaction(SIGCHLD, &act, 0); /* 시그널 핸들러 등록 */
  if(state != 0)
    error_handling("sigaction() error")
  
  serv_sock=socket(PF_INET, SOCK_STREAM, 0);   

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_addr.sin_port=htons(atoi(argv[1]));

  if( bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) )
    error_handling("bind() error");
  if( listen(serv_sock, 5))
    error_handling("listen() error");

  while(1){    
    addr_size=sizeof(clnt_addr);    

    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
    if(clnt_sock == -1)
      continue;
 
    /* 클라이언트와의 연결을 위해 프로세스 생성 */
    if( (pid=fork()) ==-1) { /* fork 실패 시 */
      close(clnt_sock);
      continue;
    }
    else if( pid>0 ) {       /* 부모 프로세스 실행 영역 */
      int result;
      puts("연결 생성");
      close(clnt_sock);

      /* 이제부터 게임 시작 */
      write(1, intro, sizeof(intro));
      read(0, buffer, BUFSIZE);                 /* 서버로부터 선택(가위 바위 보) 수신 */
      read(fd1[0], &buffer[1], BUFSIZE-1);      /* 자식 프로세스로부터 클라이언트 선택 수신 */
      
      result = who_win(buffer[0], buffer[1]);	/* 승자를 가린다 */
      if(result==0){			  /* 비긴 경우 */
		write(1, no_winner, sizeof(no_winner));
		write(fd2[1], no_winner, sizeof(no_winner));
      }
      else if( result == 1){	  /* 서버가 이긴 경우 */
		write(1, win, sizeof(win));
		write(fd2[1], lose, sizeof(lose));
      }
      else{						  /* 클라이언트가 이긴 경우 */
		write(1, lose, sizeof(lose));
		write(fd2[1], win, sizeof(win)); 
      }
    }
    else {                  /* 자식 프로세스 실행 영역 */
      close(serv_sock);

      write(clnt_sock, intro, sizeof(intro));
      read(clnt_sock, buffer, BUFSIZE);			/* 클라이언트의 선택 수신 */
      write(fd1[1], buffer, 1);					/* 부모 프로세스로 선택 전송 */
      str_len = read(fd2[0], buffer, BUFSIZE);  /* 부모 프로세스로부터 결과 수신 */
      write(clnt_sock, buffer, str_len);		/* 클라이언트로 결과 전송 */
      
      puts("연결 종료");
      close(clnt_sock);
      exit(0);
    }
  }
  return 0;
}

void z_handler(int sig)
{
  pid_t pid;
  int rtn;

  pid=waitpid(-1, &rtn, WNOHANG);
  printf("소멸 된 좀비의 프로세스 ID : %d \n", pid);
  printf("리턴 된 데이터 : %d \n\n", WEXITSTATUS(rtn)); 
}

int who_win(int a, int b) /* 승자를 가리기 위한 데이터 비교 */
{
  if(a==b)
    return 0;
  else if((a+1)%3==b%3)
    return 1;
  else
    return -1;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}