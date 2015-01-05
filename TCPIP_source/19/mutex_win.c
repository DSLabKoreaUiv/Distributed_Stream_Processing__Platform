/*
 * mutex_win.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>    /* _beginthreadex, _endthreadex */

DWORD WINAPI ThreadIncrement(void *arg);
void ErrorHandling(char *message);

char thread1[] = "A Thread";
char thread2[] = "B Thread";

int number=0;
HANDLE hMutex;

int main(int argc, char **argv) 
{	
	HANDLE  hThread1, hThread2;
	DWORD dwThreadID1, dwThreadID2;

	hMutex = CreateMutex(NULL, FALSE, NULL);

	if(hMutex==NULL){
		puts("뮤텍스 오브젝트 생성 실패");
		exit(1);
	}
	
	hThread1 = (HANDLE)_beginthreadex(NULL, 0, ThreadIncrement, (void*)thread1, 0, (unsigned *)&dwThreadID1);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, ThreadIncrement, (void*)thread2, 0, (unsigned *)&dwThreadID2);

	if(hThread1==0 || hThread2==0) {
		puts("쓰레드 생성 오류");
		exit(1);
	}

	if(WaitForSingleObject(hThread1, INFINITE)==WAIT_FAILED)
		ErrorHandling("쓰레드 wait 오류");
		
	if(WaitForSingleObject(hThread2, INFINITE)==WAIT_FAILED)
		ErrorHandling("쓰레드 wait 오류");
  
	printf("최종 number : %d \n", number);
	
	CloseHandle(hMutex); //Mutex 오브젝트 소멸
    return 0;
}

DWORD WINAPI ThreadIncrement(void *arg) 
{
    int i;

    for(i=0; i<5; i++) {
       WaitForSingleObject(hMutex, INFINITE); //Mutex를 얻는다.
       Sleep(100);
       number++;
       printf("실행 : %s,  number : %d \n", (char*)arg, number);
       ReleaseMutex(hMutex);// Mutex를 반환한다.
    }  
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}