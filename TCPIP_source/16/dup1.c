/*
 * dup1.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>

int main(void)
{
  int fd;
  fd=dup(1); /* standard out */
  printf("����� ���� ��ũ���� : %d\n", fd);
  write(fd, "����� ���� ��ũ���Ϳ� ���� ��� \n",36);

  return 0;
}
