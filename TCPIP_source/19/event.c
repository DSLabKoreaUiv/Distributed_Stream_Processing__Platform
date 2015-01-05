/*
 * event.c
 * Written by SW. YOON
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>    /* _beginthreadex, _endthreadex */

DWORD WINAPI NumberOfA(void *arg);
DWORD WINAPI NumberOfOthers(void *arg);

void ErrorHandling(char *message);
char String[100];
HANDLE hEvent;

int main(int argc, char **argv) 
{	
	HANDLE  hThread1, hThread2;
	DWORD dwThreadID1, dwThreadID2;

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(hEvent==NULL){
		puts("Event 오브젝트 생성 실패");
		exit(1);
	}
	
	hThread1 = (HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, (unsigned *)&dwThreadID1);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, (unsigned *)&dwThreadID2);

	if(hThread1==0 || hThread2==0) {
		puts("쓰레드 생성 오류");
		exit(1);
	}	

	fputs("문자열을 입력 하세요 : ", stdout); 
	fgets(String, 30, stdin);
	SetEvent(hEvent);

	if(WaitForSingleObject(hThread1, INFINITE)==WAIT_FAILED)
		ErrorHandling("쓰레드 wait 오류");
		
	if(WaitForSingleObject(hThread2, INFINITE)==WAIT_FAILED)
		ErrorHandling("쓰레드 wait 오류");

 	CloseHandle(hEvent); //Event 오브젝트 소멸
    return 0;
}

DWORD WINAPI NumberOfA(void *arg) 
{
  int i;
  int count=0;
  
  WaitForSingleObject(hEvent, INFINITE); //Event를 얻는다.
  for(i=0; String[i]!=0; i++) {
	  if(String[i]=='A')
		  count++;
  }

  printf("A 문자의 수 : %d\n", count);
  return 0;
}

DWORD WINAPI NumberOfOthers(void *arg) 
{
  int i;
  int count=0;
  
  WaitForSingleObject(hEvent, INFINITE); //Event를 얻는다.
  for(i=0; String[i]!=0; i++) {
	  if(String[i]!='A')
		  count++;
  }

  printf("A 이외의 문자 수 : %d\n", count-1);
  return 0;
}

void ErrorHandling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
