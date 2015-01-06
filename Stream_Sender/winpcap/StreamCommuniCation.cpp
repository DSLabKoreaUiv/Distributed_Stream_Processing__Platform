#include "StreamCommuniCation.h"
#include <string>

CStreamCommuniCation::CStreamCommuniCation(char* ip,int port)
{
	multiTTL=TTL;

		if (WSAStartup(MAKEWORD(2,2),&WSAData) !=0)     //winsock.dll ?ех?
	{
		printf("WSAStartup() error!");
		  exit(1);
	}

	s=socket(PF_INET, SOCK_DGRAM, 0);

	if(s==INVALID_SOCKET)
	{
		printf("WSAStartup() error!");
		  exit(1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family=AF_INET;	
	addr.sin_port=htons(port);						//port
	addr.sin_addr.S_un.S_addr=inet_addr(ip);		//ip


	 int state=setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (char*) &multiTTL, sizeof(multiTTL));
  if(state==SOCKET_ERROR)
  {
   	printf("setsockopt() error");
	  exit(1);
  }

}
	


CStreamCommuniCation::~CStreamCommuniCation(void)
{	
	closesocket(s);
	WSACleanup();	
}



void CStreamCommuniCation:: Send(char* message)
{

		//send(s,message,strlen(message),0);
	  sendto(s, message, strlen(message), 0, (SOCKADDR*)&addr, sizeof(addr));
	
}


