#include "StreamCommuniCation.h"
#include <string>




CStreamCommuniCation::CStreamCommuniCation(char* ip,int port)
{

	multiTTL=TTL;

		if (WSAStartup(MAKEWORD(2,2),&WSAData) !=0)     //winsock.dll
	{
	
	}

	s=socket(PF_INET, SOCK_DGRAM, 0);

	if(s==INVALID_SOCKET)
	{
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family=AF_INET;	
	addr.sin_port=htons(port);						//port 
	addr.sin_addr.S_un.S_addr=inet_addr(ip);		//ip 

	
	 state=setsockopt(s, IPPROTO_IP, 30, (char*) &multiTTL, sizeof(multiTTL));

  if(state==SOCKET_ERROR)
  {
  
	  printf("setsockopt() error");
  }
   


	if(connect(s,(struct sockaddr*)&addr,sizeof(addr)) ==SOCKET_ERROR)
	{
		closesocket(s);
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
