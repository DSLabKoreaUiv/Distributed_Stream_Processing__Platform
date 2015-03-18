// connectArduino.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string.h>

#define BUFSIZE 100
#define PORT 9000
typedef struct //소켓 정보를 구조체화.
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct // 소켓의 버퍼 정보를 구조체화.
{
	OVERLAPPED overlapped;
	char buffer[BUFSIZE];
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

unsigned int WINAPI CompletionThread(LPVOID CompletionPortIO);
void ErrorHandling(char *message);

int main(int argc, char** argv)
{
	WSADATA	wsaData;
	HANDLE hCompletionPort;	
	SYSTEM_INFO SystemInfo;
	SOCKADDR_IN servAddr;
	LPPER_IO_DATA PerIoData;
	LPPER_HANDLE_DATA PerHandleData;

	SOCKET hServSock;
	DWORD RecvBytes;
	DWORD i, Flags;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Load Winsock 2.2 DLL 
		ErrorHandling("WSAStartup() error!"); 

	//1. Completion Port 생성.
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&SystemInfo);

	//2. Completion Port 에서 입출력 완료를 대기 하는 쓰레드를 CPU 개수 만큼 생성.
	for(i=0; i < 10 ; i++)
		_beginthreadex(NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port=htons(PORT);

	bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hServSock, 5);
	
	while(TRUE)
	{		
		SOCKET hClntSock;
		SOCKADDR_IN clntAddr;		
		int addrLen=sizeof(clntAddr);
		
		hClntSock=accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen);		
		//printf("Accept\n");

		// 연결 된 클라이언트의 소켓 핸들 정보와 주소 정보를 설정.
		PerHandleData=(LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));		
		PerHandleData->hClntSock=hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

		//3. Overlapped 소켓과 CompletionPort의 연결.
		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0);
		
		// 연결 된 클라이언트를 위한 버퍼를 설정 하고 OVERLAPPED 구조체 변수 초기화.
		PerIoData=(LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));		
		PerIoData->wsaBuf.len=BUFSIZE;
		PerIoData->wsaBuf.buf=PerIoData->buffer;
		Flags=0;

		//4. 중첩된 데이터 입력.
		WSARecv(PerHandleData->hClntSock,	// 데이터 입력 소켓.
				&(PerIoData->wsaBuf),		// 데이터 입력 버퍼 포인터.
				1,							// 데이터 입력 버퍼의 수.
				&RecvBytes,					
				&Flags, 
				&(PerIoData->overlapped),	// OVERLAPPED 구조체 포인터.
				NULL
				);			
	}
	return 0;
}

//입 출력 완료에 따른 쓰레드의 행동 정의 
unsigned int WINAPI CompletionThread(LPVOID pComPort)
{
	HANDLE hCompletionPort =(HANDLE)pComPort;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData; 
	LPPER_IO_DATA PerIoData;
	DWORD flags;
	char tmp[BUFSIZE];

	while(1)
	{

		// 5. 입출력이 완료 된 소켓의 정보 얻음. 
		GetQueuedCompletionStatus(hCompletionPort,			 // Completion Port
								  &BytesTransferred,		 // 전송 된 바이트 수
								  (LPDWORD)&PerHandleData,
								  (LPOVERLAPPED*)&PerIoData, // OVERLAPPED 구조체 포인터.
								  INFINITE
								  );

		PerIoData->buffer[BytesTransferred]=NULL;
		printf("%s", (char*)PerIoData->buffer);

		if(BytesTransferred==0) //EOF 전송시.
		{
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(PerIoData);
			continue;		
		}		
		
			
		
		// 6. 메시지! 클라이언트로 에코.
		PerIoData->wsaBuf.len=BytesTransferred;
		WSASend(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL);
	
		// RECEIVE AGAIN
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len=BUFSIZE;
		PerIoData->wsaBuf.buf=PerIoData->buffer;

		flags=0;
		WSARecv(PerHandleData->hClntSock, 
				&(PerIoData->wsaBuf), 
				1, 
				NULL, 
				&flags, 
				&(PerIoData->overlapped), 
				NULL
				);				
		//printf("%s \n", PerIoData->wsaBuf);
	}
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

