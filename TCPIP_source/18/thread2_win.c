/*
 * thread2_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>    /* _beginthreadex, _endthreadex */

DWORD WINAPI ThreadFunction(void *arg);

int main(int argc, char **argv) 
{
	HANDLE hThread;
	DWORD dwThreadID;
	DWORD dw;

    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned *)&dwThreadID);
    if(hThread == 0){
      puts("_beginthreadex() error");
      exit(1);
    }

    printf("생성된 쓰레드의 핸들 : %d \n", hThread);
	printf("생성된 쓰레드의 ID : %d \n", dwThreadID);
    Sleep(3000); /*millisecond 단위 */

	/* 쓰레드 종료 시까지 main함수의 실행을 지연 */
    dw = WaitForSingleObject(hThread, INFINITE);
    if(dw == WAIT_FAILED) {
      puts("쓰레드 wait 오류");
      exit(1);
    }
    printf("main함수 종료, %s 종료\n", dw==WAIT_OBJECT_0?"정상":"비 정상");
    return 0;
}

DWORD WINAPI ThreadFunction(void *arg) 
{
  int i;
  for(i=0; i<5; i++){
      Sleep(2000);
      puts("쓰레드 실행 중");
  }
  return 0;
}