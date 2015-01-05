/*
 * make_socket.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char **argv)
{
	int tcp_socket;
	int udp_socket;
	
	/* 楷搬瘤氢 TCP 家南 积己 */
	tcp_socket=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(tcp_socket==-1)
		error_handling("socket() error");
	
	/* 厚 楷搬瘤氢 UDP 家南 积己 */
	udp_socket=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(udp_socket==-1)
		error_handling("socket() error");
	
	printf("积己等 TCP 家南狼 颇老 叼胶农赋磐 : %d \n", tcp_socket);
	printf("积己等 UDP 家南狼 颇老 叼胶农赋磐 : %d \n", udp_socket);
	
	close(tcp_socket);
	close(udp_socket);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
