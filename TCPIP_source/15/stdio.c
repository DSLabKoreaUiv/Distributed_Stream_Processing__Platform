/*
 * stdio.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>

void error_handling (char *message);

int main(void)
{
  FILE *fp; 

  /* ǥ�� ����� �Լ��� ���� ���� ���� */
  fp = fopen("test.dat", "w");
  if(fp == NULL)
    error_handling("file open error");

  fputs("Network programming\n\n", fp);
  fclose(fp);
  return 0;
}

void error_handling (char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
