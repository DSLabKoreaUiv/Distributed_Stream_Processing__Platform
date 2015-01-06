#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#define TTL 64
class CStreamCommuniCation
{
public:

	CStreamCommuniCation(char* ip,int port);
	~CStreamCommuniCation(void);

	int multiTTL;
	WSADATA WSAData;
	SOCKADDR_IN addr;
	SOCKET s;

	void Send(char* message);

};

