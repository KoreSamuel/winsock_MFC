#include "stdafx.h"
#include "SocketParent.h"


CSocketParent::CSocketParent(void)
{
	
}


CSocketParent::~CSocketParent(void)
{
}

int CSocketParent::Create(int port)
{
	SOCKADDR_IN addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	return bind(m_listen,(SOCKADDR *)&addr,sizeof(addr));
}

void CSocketParent::Listen(int n)
{
	listen(m_listen,n);
}


void CSocketParent :: Close()
{
	closesocket(m_listen);
}

void CSocketParent :: Init()
{
	WSAStartup(MAKEWORD(2,2),&m_wsaData);
	m_listen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
}