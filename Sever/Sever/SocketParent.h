#pragma once
class CSocketParent
{
public:
	CSocketParent(void);
	~CSocketParent(void);

public:
	WSADATA m_wsaData;
	SOCKET m_listen;
	char m_szBuffer[4096];
	CString m_user;
	CString m_passw;

	int Create(int port);
	void Listen(int n);
	void Close();
	void Init();
	virtual void Receive(int num,int i){};
};

