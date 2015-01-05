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
    printf("fork 실패 프로세스 id : %d \n", pid);

  printf("fork 성공 프로세스 id : %d \n", pid);

  if(pid==0) /* 자식 프로세스라면 */
    data+=10;
  else       /* 부모 프로세스라면 */
  {
    data-=10;
    child=wait(&state); /* 자식 프로세스의 종료 대기 */
    printf("자식 프로세스 ID = %d \n", child);
    printf("리턴 값 = %d \n", WEXITSTATUS(state));
    sleep(20); /* 프로세스 상태 확인을 위해서 */ 
  }

  printf("data : %d \n", data);
  return 0;
}
