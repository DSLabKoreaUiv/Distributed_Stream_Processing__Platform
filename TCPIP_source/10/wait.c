/*
 * wait.c
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
    printf("fork ���� ���μ��� id : %d \n", pid);

  printf("fork ���� ���μ��� id : %d \n", pid);

  if(pid==0) /* �ڽ� ���μ������ */
    data+=10;
  else       /* �θ� ���μ������ */
  {
    data-=10;
    child=wait(&state); /* �ڽ� ���μ����� ���� ��� */
    printf("�ڽ� ���μ��� ID = %d \n", child);
    printf("���� �� = %d \n", WEXITSTATUS(state));
    sleep(20); /* ���μ��� ���� Ȯ���� ���ؼ� */ 
  }

  printf("data : %d \n", data);
  return 0;
}
