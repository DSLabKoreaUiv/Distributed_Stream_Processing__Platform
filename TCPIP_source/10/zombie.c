/*
 * zombie.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
  pid_t pid;
  int data=10;

  pid=fork();
  if(pid<0)
    printf("fork ���� ���μ��� id : %d \n", pid);

  printf("fork ���� ���μ��� id : %d \n", pid);

  if(pid==0) /* �ڽ� ���μ������ */
    data+=10;
  else       /* �θ� ���μ������ */
  {
    data-=10;
    sleep(20); /* 20�� ���� ���� ���¿� ���� */ 
  }

  printf("data : %d \n", data);
  return 0;
}
