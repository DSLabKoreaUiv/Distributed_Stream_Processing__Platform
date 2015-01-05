/*
 * sigint2.c
 * Written by SW. YOON
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig);

int main(void)
{
  int state;
  int num=0;
 
  struct sigaction act;      /* �ڵ鷯 ����ü ���� */
  act.sa_handler=handler;    /* �ڵ鷯 �Լ� ����   */
  sigemptyset(&act.sa_mask); /* sm_mask�� ��� ��Ʈ�� 0���� ���� */
  act.sa_flags=0;            

  state=sigaction(SIGINT, &act, 0); /* �ñ׳� �ڵ鷯 ��� */
  if(state != 0){
    puts("sigaction() error ");
    exit(1);
  }

  while(1){
    printf("%d : ����� \n", num++);
    sleep(2);
    if(num>5) break;
  }
  return 0;
}

/* �ñ׳� ó�� �Լ� */
void handler(int sig)
{
  printf("���޵� �ñ׳��� %d \n", sig); 
}




