/*
 * stream_handle.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void error_handling (char *message);

int main(void)
{
  int fildes;
  FILE *fp;

  fildes = open("data.dat", O_WRONLY|O_CREAT|O_TRUNC);
  if(fildes == -1)
    error_handling("file open error");

  printf("First file descriptor : %d \n", fildes); 

  /* 파일 디스크립터를 이용하여 파일포인터 생성 */
  fp=fdopen(fildes, "w");
  fputs("TCP/IP SOCKET PROGRAMMING\n\n", fp);
  
  printf("Second file descriptor : %d \n\n", fileno(fp));
  fclose(fp);

  return 0;
}

void error_handling (char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

