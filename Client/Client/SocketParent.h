#pragma once

class CSocketParent
{
public:
	CSocketParent(void);
	~CSocketParent(void);
public:
	SOCKET m_host;
	char m_szBuffer[4096];
	void Close(void);
	SOCKET Create(void);
	virtual int Connect(char *ipAddress,int port) = 0;
	virtual void Send(char* sendData,int len) = 0;
	virtual void Receive(int num) = 0;
};

