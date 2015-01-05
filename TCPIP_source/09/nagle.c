/*
 * nagle.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char **argv)
{
	int sock;
	int state, opt_val, opt_len;
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	/* ����Ʈ nagle �˰��� ���� ���� Ȯ�� */
	state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &opt_val, &opt_len);
	if(state)
	{
		puts("getsockopt() error!");
		exit(1);
	}
	printf("����Ʈ nagle �˰��� : %s \n", opt_val ? "����":"����");
	
	/* nagle �˰����� DISABLE ��Ų�� */
	opt_val=TRUE;
	state = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(opt_val));
	if(state)
	{
		puts("setsockopt() error!");
		exit(1);
	}
	
	/* ����� nagle �˰��� Ȯ�� */
	getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &opt_val, &opt_len);
	printf("����� nagle �˰��� : %s \n",opt_val ? "����":"����");
	
	close(sock);
	return 0;
}