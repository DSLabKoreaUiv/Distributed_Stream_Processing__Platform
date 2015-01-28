// connectArduino.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

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

