/*
 * thread1_win.c
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

    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned *)&dwThreadID);
    if(hThread == 0){
      puts("_beginthreadex() error");
      exit(1);
    }
    printf("������ �������� �ڵ� : %d \n", hThread);
	printf("������ �������� ID : %d \n", dwThreadID);
    Sleep(3000); /*millisecond ���� */
    puts("main�Լ� ����");

    return 0;
}

DWORD WINAPI ThreadFunction(void *arg) 
{
  int i; 
  for(i=0; i<5; i++){
      Sleep(2000);
      puts("������ ���� ��");
  }
  return 0;
}