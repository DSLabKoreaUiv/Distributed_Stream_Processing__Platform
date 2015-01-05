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
  else if(pid==0){  /*자식 프로세스의 경우 */
    write(fd[1], "Good!", 6);
    sleep(2);
    read(fd[0], buffer, BUFSIZE);
    printf("자식 프로세스 출력 : %s \n\n",  buffer);
   
  }
  else{             /* 부모 프로세스의 경우 */
    read(fd[0], buffer, BUFSIZE);
    printf("부모 프로세스 출력 : %s \n", buffer); 
    write(fd[1], "Really Good", 12);
    sleep(3);		/* 큰의미 없음 : 출력 좋게 하려고 */
  }

  return 0;
}