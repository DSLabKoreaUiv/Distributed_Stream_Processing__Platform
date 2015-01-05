/*
 * pipe2.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 30

int main(int argc, char **argv)
{
  int fd[2];
  char buffer[BUFSIZE];
  pid_t pid;
  int state;

  state = pipe(fd);
  if(state == -1){
    puts("pipe() error");
    exit(1);
  }

  pid = fork();
  if(pid == -1){
    puts("fork() error");
    exit(1);
  }
  else if(pid==0){  /*�ڽ� ���μ����� ��� */
    write(fd[1], "Good!", 6);
    sleep(2);
    read(fd[0], buffer, BUFSIZE);
    printf("�ڽ� ���μ��� ��� : %s \n\n",  buffer);
   
  }
  else{             /* �θ� ���μ����� ��� */
    read(fd[0], buffer, BUFSIZE);
    printf("�θ� ���μ��� ��� : %s \n", buffer); 
    write(fd[1], "Really Good", 12);
    sleep(3);		/* ū�ǹ� ���� : ��� ���� �Ϸ��� */
  }

  return 0;
}