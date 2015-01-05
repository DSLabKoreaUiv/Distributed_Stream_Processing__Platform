/*
 * sysio.c
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
  char str[] = "socket programming\n\n";

  /* 시스템 함수를 통한 파일의 생성 */
  fildes = open("data.daa", O_WRONLY|O_CREAT|O_TRUNC);
  if(fildes == -1)
    error_handling("file open error");

  write(fildes, str, sizeof(str)-1);
  close(fildes);
  return 0;
}

void error_handling (char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

