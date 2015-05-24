#pragma once

class CClientDlg;
#include "SocketParent.h"

class CConnSocket : public CSocketParent
{
public:
	CConnSocket(void);
	~CConnSocket(void);

public:
	SOCKADDR_IN servAddr;  
	CClientDlg * m_parent;
	void SetParent(CClientDlg * parent);
	int Connect(char *ipAddress,int port);
	void Send(char* sendData,int len);
	void Receive(int num);

};

