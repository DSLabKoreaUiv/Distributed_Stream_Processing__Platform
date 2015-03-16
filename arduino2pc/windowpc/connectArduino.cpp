// connectArduino.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <string.h>

#define BUFSIZE 100
#define PORT 9000
typedef struct //���� ������ ����üȭ.
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct // ������ ���� ������ ����üȭ.
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

	//1. Completion Port ����.
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&SystemInfo);

	//2. Completion Port ���� ����� �ϷḦ ��� �ϴ� �����带 CPU ���� ��ŭ ����.
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

		// ���� �� Ŭ���̾�Ʈ�� ���� �ڵ� ������ �ּ� ������ ����.
		PerHandleData=(LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));		
		PerHandleData->hClntSock=hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

		//3. Overlapped ���ϰ� CompletionPort�� ����.
		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0);
		
		// ���� �� Ŭ���̾�Ʈ�� ���� ���۸� ���� �ϰ� OVERLAPPED ����ü ���� �ʱ�ȭ.
		PerIoData=(LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));		
		PerIoData->wsaBuf.len=BUFSIZE;
		PerIoData->wsaBuf.buf=PerIoData->buffer;
		Flags=0;

		//4. ��ø�� ������ �Է�.
		WSARecv(PerHandleData->hClntSock,	// ������ �Է� ����.
				&(PerIoData->wsaBuf),		// ������ �Է� ���� ������.
				1,							// ������ �Է� ������ ��.
				&RecvBytes,					
				&Flags, 
				&(PerIoData->overlapped),	// OVERLAPPED ����ü ������.
				NULL
				);			
	}
	return 0;
}

//�� ��� �Ϸῡ ���� �������� �ൿ ���� 
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

		// 5. ������� �Ϸ� �� ������ ���� ����. 
		GetQueuedCompletionStatus(hCompletionPort,			 // Completion Port
								  &BytesTransferred,		 // ���� �� ����Ʈ ��
								  (LPDWORD)&PerHandleData,
								  (LPOVERLAPPED*)&PerIoData, // OVERLAPPED ����ü ������.
								  INFINITE
								  );

		//strcpy_s(tmp, (char*)PerIoData->wsaBuf.buf);
		//tmp[PerIoData->wsaBuf.len]=NULL;
	
		//sprintf_s(tmp, "%s", (char *)PerIoData->wsaBuf.buf);
		//tmp[PerIoData->wsaBuf.len]=NULL;
//		printf("%s", tmp);
		//printf("%s, %d", (char*)PerIoData->wsaBuf.buf, PerIoData->wsaBuf.len);
//		PerIoData->buffer[7]=NULL;
		//strcpy_s(tmp, PerIoData->buffer);
		PerIoData->buffer[BytesTransferred]=NULL;
		printf("%s", (char*)PerIoData->buffer);

		if(BytesTransferred==0) //EOF ���۽�.
		{
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(PerIoData);
			continue;		
		}		
		
			
		
		// 6. �޽���! Ŭ���̾�Ʈ�� ����.
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


/*
#include "stdafx.h"
#include <winsock.h>

#define MAX_PACKETLEN 100 
#define PORT 9000

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	int status;
	int SockLen; 
    int Readn,Writen; 
    SOCKET EndpointSocket, ClientSocket; 
    struct sockaddr_in SockInfo, ClientSockInfo; 
    char ReadBuffer[MAX_PACKETLEN]; 

	if(WSAStartup(MAKEWORD(2,2),&wsaData)!= 0) 
    { 
        printf("error\r\n"); 
        return 0; 
    } 

	EndpointSocket = socket( AF_INET, SOCK_STREAM, 0 ); 
    if( EndpointSocket == INVALID_SOCKET ) 
        return 1; 
 
	printf("Success socket create\r\n"); 
    ZeroMemory(&SockInfo, sizeof( struct sockaddr_in )); 
	    
	
	SockInfo.sin_family = AF_INET; 
    SockInfo.sin_port = htons( PORT ); 
    SockInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY); 
 
    status = bind( EndpointSocket, (struct sockaddr*)&SockInfo, sizeof( struct sockaddr_in) ); 
    if( status == SOCKET_ERROR)  
    { 
        printf("Bind Error\n"); 
        return 0; 
    } 
    if( SOCKET_ERROR == listen( EndpointSocket, 5 )) 
    { 
        printf("listen Error\n"); 
        return 0; 
    } 

	ZeroMemory( &ClientSockInfo, sizeof( struct sockaddr_in ) ); 
	SockLen = sizeof(struct sockaddr_in); 
	ClientSocket = accept( EndpointSocket, (struct sockaddr*)&ClientSockInfo, &SockLen ); 
	
	if(ClientSocket == INVALID_SOCKET) 
    { 
        printf("Accept Error\n"); 
        closesocket(EndpointSocket); 
        WSACleanup(); 
        return 1; 
    } 
    printf("Accept Client\n"); 

    while(1) 
    { 

        
        Readn = recv( ClientSocket, ReadBuffer, MAX_PACKETLEN,0 );
		ReadBuffer[Readn]=NULL;
		printf("%s %d\n", ReadBuffer, Readn);
        if( Readn > 0 ) 
        { 
            Writen = send( ClientSocket, ReadBuffer, Readn, 0 ); 
        } 
        else 
        { 
            printf("read Error\n"); 
        } 
        //closesocket(ClientSocket);  
    } 
    closesocket( EndpointSocket );  
    WSACleanup(); 


	return 0;
}
*/