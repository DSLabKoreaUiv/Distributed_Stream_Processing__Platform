/*
 * handle_stream.c
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

  /* 시스템 함수를 통한 파일 생성 */
  fildes = open("data.dat", O_WRONLY|O_CREAT|O_TRUNC);
  if(fildes == -1)
    error_handling("file open error");

  /* 파일 디스크립터를 이용하여 파일포인터 생성 */
  fp=fdopen(fildes, "w");

  fputs("Network C programming\n\n", fp);
  fclose(fp);

  return 0;
}

void error_handling (char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}