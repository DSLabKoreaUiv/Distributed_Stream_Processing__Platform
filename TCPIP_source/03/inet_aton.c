/*
 * inet_aton.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <arpa/inet.h>

void error_handling(char *message);

int main(int argc, char **argv)
{
  char * addr = "1.2.3.4";
  struct sockaddr_in addr_inet;

  if( !inet_aton(addr, &addr_inet.sin_addr))
    error_handling("Conversion Error");
  printf("Unsigned long addr(network ordered) : %x \n\n", addr_inet.sin_addr.s_addr);

  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
