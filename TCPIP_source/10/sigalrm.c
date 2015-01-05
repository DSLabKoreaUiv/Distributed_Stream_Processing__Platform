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
  
  state=sigaction(SIGALRM, &act, 0); /* 시그널 핸들러 등록 */
  if(state != 0){
    puts("sigaction() error ");
    exit(1);
  }
  
  alarm(5); /* 5초 후에 SIGALRM 시그널 예약 */
  while(1){
    puts("대기중");
    sleep(2);
  }
  return 0;
}

void timer(int sig)
{
  puts("예약 하신 시간이 되었습니다!! \n");
  exit(0);
}