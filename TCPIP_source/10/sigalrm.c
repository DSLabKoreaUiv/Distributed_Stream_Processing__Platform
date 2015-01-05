/*
 * sigalrm.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void timer(int sig);

int main(int argc, char **argv)
{
  int state;

  struct sigaction act;
  act.sa_handler=timer;
  sigemptyset(&act.sa_mask);
  act.sa_flags=0;
  
  state=sigaction(SIGALRM, &act, 0); /* �ñ׳� �ڵ鷯 ��� */
  if(state != 0){
    puts("sigaction() error ");
    exit(1);
  }
  
  alarm(5); /* 5�� �Ŀ� SIGALRM �ñ׳� ���� */
  while(1){
    puts("�����");
    sleep(2);
  }
  return 0;
}

void timer(int sig)
{
  puts("���� �Ͻ� �ð��� �Ǿ����ϴ�!! \n");
  exit(0);
}