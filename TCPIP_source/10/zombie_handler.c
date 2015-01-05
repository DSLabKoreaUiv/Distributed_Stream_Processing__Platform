/*
 * zombie_handler.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void z_handler(int sig);

int main(int argc, char **argv)
{
  pid_t pid;
  int state, i;

  struct sigaction act;
  act.sa_handler=z_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags=0;
  
  state=sigaction(SIGCHLD, &act, 0); /* 시그널 핸들러 등록 */
  if(state != 0){
    puts("sigaction() error ");
    exit(1);
  }
 
  pid=fork();
  if(pid==0) {
    printf("자식 프로세스 생성 : %d \n", getpid());
    exit(3);
  }
  else {
    sleep(3);
  }

  return 0;  
}

void z_handler(int sig)
{
  pid_t pid;
  int rtn;

  while( (pid=waitpid(-1, &rtn, WNOHANG)) > 0){
    printf("소멸된 좀비의 프로세스 ID : %d \n", pid);
    printf("리턴된 데이터 : %d \n\n", WEXITSTATUS(rtn));
  }
}