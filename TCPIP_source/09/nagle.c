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
	
	/* 디폴트 nagle 알고리즘 설정 상태 확인 */
	state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &opt_val, &opt_len);
	if(state)
	{
		puts("getsockopt() error!");
		exit(1);
	}
	printf("디폴트 nagle 알고리즘 : %s \n", opt_val ? "비설정":"설정");
	
	/* nagle 알고리즘을 DISABLE 시킨다 */
	opt_val=TRUE;
	state = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(opt_val));
	if(state)
	{
		puts("setsockopt() error!");
		exit(1);
	}
	
	/* 변경된 nagle 알고리즘 확인 */
	getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &opt_val, &opt_len);
	printf("변경된 nagle 알고리즘 : %s \n",opt_val ? "비설정":"설정");
	
	close(sock);
	return 0;
}