/*
 * dup2.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>

int main(void)
{
  int fd;
  int state;
  fd=dup(1); /* standard out */
  printf("����� ���� ��ũ���� : %d\n", fd);
  write(fd, "����� ���� ��ũ���Ϳ� ���� ��� \n",36);

  state = close(1);
  if(state ==-1){
    puts("���� �߻�\n");
    exit(1);
  }

  write(fd, "����� ���� ��ũ���Ϳ� ���� ��� \n",36);
  return 0;
}
