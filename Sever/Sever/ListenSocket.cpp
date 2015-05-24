#include "StdAfx.h"
#include "ListenSocket.h"
#include "ConnSocket.h"
#include "SeverDlg.h"

CListenSocket::CListenSocket(void)
{
}


CListenSocket::~CListenSocket(void)
{
}


void CListenSocket::SetParent(CSeverDlg * parent)
{
	m_parent = parent;
}

