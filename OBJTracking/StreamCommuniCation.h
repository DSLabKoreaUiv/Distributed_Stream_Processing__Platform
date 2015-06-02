#pragma once
#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <stdio.h>
#define TTL 64

class CStreamCommunication
{
public:
	CStreamCommunication(char* ip,int port);
	~CStreamCommunication(void);

	int multiTTL;
	WSADATA WSAData;
	SOCKADDR_IN addr;
	SOCKET s;
	
	void Send(char* message);

};

