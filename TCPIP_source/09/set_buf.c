/*
 * set_buf.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char **argv)
{
	int sock;

	int snd_buf=500;
	int rcv_buf=1000;  
	
	int state;
	socklen_t len;
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, sizeof(rcv_buf));
	if(state)
		error_handling("getsockopt() error!");
	
	state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, sizeof(snd_buf));
	if(state)
		error_handling("getsockopt() error!");
	
	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &len);
	if(state)
		error_handling("setsockopt() error!");
	
	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, &len);
	if(state)
		error_handling("setsockopt() error!");
	
	printf("데이터 입력을 위한 소켓의 버퍼 크기 : %d \n", rcv_buf);
	printf("데이터 출력을 위한 소켓의 버퍼 크기 : %d \n", snd_buf);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}



