/*
 * bind_sock.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(void)
{
	int serv_sock;
	char *serv_ip="127.0.0.1";
	char *serv_port="9190";
	struct sockaddr_in serv_addr;
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);   /* 소켓 생성 */
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(serv_ip); //htons(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(serv_port));
	
	if( bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 ) /* 소켓에 주소를 할당 */
		error_handling("bind() error");
	
	printf("파일 디스크립터 %d 의 소켓에 주소 할당까지 완료!\n\n", serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}