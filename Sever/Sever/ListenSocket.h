#pragma once

class CSeverDlg;
#include "SocketParent.h"

class CListenSocket : public CSocketParent
{
public:
	CListenSocket(void);
	~CListenSocket(void);
public: 
	CSeverDlg *m_parent;
	void SetParent(CSeverDlg * parent);
};

