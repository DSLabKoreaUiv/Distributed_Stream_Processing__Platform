/*
 * get_buf.c
 * Written by SW. YOON.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char **argv)
{
	int sock;  
	
	int snd_buf;
	int rcv_buf;
	
	int state;
	socklen_t len;
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &len);
	if(state)
		error_handling("getsockopt() error");
	
	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, &len);
	if(state)
		error_handling("getsockopt() error");
	
	printf("������ �Է��� ���� ������ ���� ũ�� : %d \n", rcv_buf);
	printf("������ ����� ���� ������ ���� ũ�� : %d \n", snd_buf);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}