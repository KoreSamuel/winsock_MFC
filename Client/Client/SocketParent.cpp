#include "stdafx.h"
#include "SocketParent.h"


CSocketParent::CSocketParent(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
}


CSocketParent::~CSocketParent(void)
{

}

SOCKET CSocketParent::Create(void)
{
	m_host = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return m_host;
}


void CSocketParent::Close(void)
{
	closesocket(m_host);
}
