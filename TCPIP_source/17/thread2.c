/*
 * thread2.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);

int main(int argc, char **argv) 
{
    int state;
    pthread_t t_id;
    void * t_return;

    state = pthread_create(&t_id, NULL, thread_function, NULL); /* ������ ���� */
    if(state != 0){
      puts("������ ���� ����");
      exit(1);
    }
    printf("������ �������� ID : %d \n", t_id);
    
    /* ������ ���� �ñ��� main�Լ��� ������ ���� */
    state = pthread_join(t_id, &t_return); /* ���� �� ���� */
    if(state != 0){
      puts("������ Join ����");
      exit(1);
    }
    printf("main�Լ� ����, ������ ���� %s", (char*)t_return);
    free(t_return);
    return 0;
}

void *thread_function(void *arg) 
{
  int i;
  char * p = (char*)malloc(20*sizeof(char));
  strcpy(p, "������ ���� ��!\n");

  for(i=0; i<3; i++){
      sleep(2);
      puts("������ ������");
  }

  return p;
}