/*
 * thread4.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMBER 10000
void *thread_increment(void *arg); 
int num=0;

int main(int argc, char **argv) 
{
  int i;   
  pthread_t thread_id[10];
  void * t_return;
   
  for(i=0; i<10; i++)      
    pthread_create(&thread_id[i], NULL, thread_increment, NULL);

  /* 생성한 모든 쓰레드 종료 시까지 main함수의 실행을 지연 */
  for(i=0; i<10; i++)
    pthread_join(thread_id[i], &t_return);
  printf("main함수 종료, num=%d \n", num);

  return 0;
}

void *thread_increment(void *arg) 
{
  int i;
  for(i=0 ; i<NUMBER; i++)
    num++;
}