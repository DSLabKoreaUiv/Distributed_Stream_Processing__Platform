/*
 * waitpid.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
  pid_t pid, child;
  int data=10;
  int state;

  pid=fork();
  if(pid<0)
    printf("fork ����, ���μ��� id : %d \n", pid);

  printf("fork ����, ���μ��� id : %d \n", pid);

  if(pid==0) /* �ڽ� ���μ������ */
  {
    data+=10;
    sleep(10); /* ���Ḧ 10�� ���� */
  }
  else       /* �θ� ���μ������ */
  {
    data-=10;
    do{
      sleep(3);
      puts("3�� ���");
      child=waitpid(-1, &state, WNOHANG);
    }while(child == 0);

    /* ������ �ڽ� ���μ��� �������� ��� */
    printf("Child process id = %d, return value = %d \n\n", child, WEXITSTATUS(state));
  }

  printf("data : %d \n", data);
  return 0;
}
