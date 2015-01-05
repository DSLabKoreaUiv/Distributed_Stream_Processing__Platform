/*
 * readv.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char **argv)
{
  struct iovec vec[2];
  char MSG1[10]={0,};
  char MSG2[10]={0,};
  int str_len;

  vec[0].iov_base=MSG1;
  vec[0].iov_len=9;

  vec[1].iov_base=MSG2;
  vec[1].iov_len=9;

  str_len=readv(0, vec, 2); 
  printf("\n�� %d ����Ʈ �Է�\n", str_len);
  printf("ù ��° �޽��� : %s \n", MSG1);
  printf("�� ��° �޽��� : %s \n", MSG2);

  return 0;
}
