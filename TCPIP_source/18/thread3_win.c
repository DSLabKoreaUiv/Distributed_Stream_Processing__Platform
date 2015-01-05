 /*
 * thread3_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //필요한 것인지 다시 한번 확인 하라...반드시 반드시...
#include <process.h>    /* _beginthreadex, _endthreadex */

DWORD WINAPI ThreadSummation(void *arg);
void ErrorHandling(char *message);

int sum=0;
int sum1[]={1, 5};
int sum2[]={6, 10};

int main(int argc, char **argv) 
{
	HANDLE hThread1, hThread2;
	DWORD dwThreadID1, dwThreadID2;

    hThread1 = (HANDLE)_beginthreadex(NULL, 0, ThreadSummation, (void*)sum1, 0, (unsigned *)&dwThreadID1);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, ThreadSummation, (void*)sum2, 0, (unsigned *)&dwThreadID2);
    if(hThread1==0 || hThread2==0) 
		ErrorHandling("쓰레드 생성 오류");

	if(WaitForSingleObject(hThread1, INFINITE)==WAIT_FAILED)
		ErrorHandling("쓰레드 wait 오류");
		
	if(WaitForSingleObject(hThread2, INFINITE)==WAIT_FAILED)
		ErrorHandling("쓰레드 wait 오류");

	printf("main함수 종료, sum = %d \n", sum);
    return 0;
}

DWORD WINAPI ThreadSummation(void *arg) 
{
  int start=((int*)arg)[0];
  int end=((int*)arg)[1];

  for( ; start<=end; start++) {
    sum+=start;
  }
  return 0;
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
