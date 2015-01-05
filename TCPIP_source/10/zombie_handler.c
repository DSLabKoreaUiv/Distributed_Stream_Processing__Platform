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
  
  state=sigaction(SIGCHLD, &act, 0); /* �ñ׳� �ڵ鷯 ��� */
  if(state != 0){
    puts("sigaction() error ");
    exit(1);
  }
 
  pid=fork();
  if(pid==0) {
    printf("�ڽ� ���μ��� ���� : %d \n", getpid());
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
    printf("�Ҹ�� ������ ���μ��� ID : %d \n", pid);
    printf("���ϵ� ������ : %d \n\n", WEXITSTATUS(rtn));
  }
}