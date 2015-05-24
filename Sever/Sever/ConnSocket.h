#pragma once

class CSeverDlg;
#include "ADOConn.h"
#include "SocketParent.h"

class CConnSocket : public CSocketParent
{
public:
	CConnSocket(void);
	~CConnSocket(void);

public:
	CSeverDlg * m_parent;
	void SetParent(CSeverDlg * parent);
	void CheckUser(int m);
	CADOConn m_ado;
	void Register(int m);
	void Receive(int num,int i);
	char * StringToChar(CString str);

	void CloseClient(void);
};

