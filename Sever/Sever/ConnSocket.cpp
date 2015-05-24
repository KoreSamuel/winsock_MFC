#include "StdAfx.h"
#include "ConnSocket.h"
#include "SeverDlg.h"

CConnSocket::CConnSocket(void)
{
}


CConnSocket::~CConnSocket(void)
{
}


void CConnSocket::SetParent(CSeverDlg * parent)
{
	m_parent = parent;
}


char * CConnSocket::StringToChar(CString str)
{
	return (LPSTR)(LPCTSTR)(str);
}



void CConnSocket::Receive(int num,int m)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString str = _T("");
	if(num != SOCKET_ERROR){
		str.Empty();
		m_szBuffer[num] = '\0';
		str = m_szBuffer;
		if(str.Left(2) == "00")
		{
			int index = str.Find(_T(" "));
			m_user = str.Mid(2,index-2);
			m_passw = str.Mid(index+1 , str.GetLength()-1);
			CheckUser(m);
		}
		else if(str.Left(2)=="01")
		{
			int index = str.Find(_T(" "));
			m_user = str.Mid(2,index-2);
			m_passw = str.Mid(index+1 , str.GetLength()-1);
			Register(m);
		}
		else if(str.Left(2) == "03")
		{
			int index = str.Find(_T(" "));
			if(index == 2)
			{
				for(int i = 1 ; i < m_parent->m_Vcon.size() ;i ++)
				{
					CString s = _T("04") + str.Mid(index +1 , str.GetLength()-1);
					send(m_parent->m_Vcon[i]->m_listen,StringToChar(s) , s.GetLength(),0);
				}
				m_parent->m_list.AddString(str.Mid(index +1 , str.GetLength()-1));
				m_parent->UpdateData(false);
			}
			else
			{
				CString name = str.Mid(2,index-2);
				for(int i = 1 ; i < m_parent->m_Vcon.size() ; i ++)
				{
					if(m_parent->m_Vcon.at(i)->m_user == name)
					{
						CString s = _T("05") + str.Mid(index+1,str.GetLength()-1);
						send(m_parent->m_Vcon[i]->m_listen,StringToChar(s) ,s.GetLength(),0);
						m_parent->m_list.AddString(str.Mid(index+1,str.GetLength()-1));
						m_parent->UpdateData(false);
						break;
					}
				}
			}
		}
		else if(str.Left(2) == "08")
		{
			int i = 0;
			vector<CSocketParent*>::iterator it = m_parent->m_Vcon.begin();
			for (;it < m_parent->m_Vcon.end(); it++,i++)
			{
				if((*it)->m_listen == m_listen)
				{
					m_parent->m_Vcon.erase(it);
					break;
				}
			}

			vector<WSAEVENT>::iterator itEvent = m_parent->m_VetEvent.begin();
			for(int j = 0 ; j < i ; j ++,itEvent++){
			}

			m_parent->m_VetEvent.erase(itEvent);

			m_parent->TreeStyle();
			for(int i = 1 ; i < m_parent->m_Vcon.size() ; i++)
			{
				if(m_parent->m_Vcon.at(i)->m_user != str.Mid(2,str.GetLength()-1))
				{
					m_parent->m_tree.InsertItem(m_parent->m_Vcon.at(i)->m_user,NULL,NULL,m_parent->m_hItem);
				}		
			}

			m_parent->m_num.Format(_T("%d"),m_parent->m_Vcon.size() - 1);
			m_parent->UpdateData(FALSE);

			CloseClient();
		}
		else
		{
			m_parent->m_list.AddString(str);
		}
	}
}


void CConnSocket::CheckUser(int m)
{
	CString string ;
	string.Format(_T("select * from Users"));
	_bstr_t strSQL = (_bstr_t)string;
	m_ado.m_pRecordset = m_ado.GetRecordset(strSQL);
	if(!m_ado.m_pRecordset->adoEOF)
		m_ado.m_pRecordset->MoveFirst();
	while(!m_ado.m_pRecordset->adoEOF)
	{
		if((CString)m_ado.m_pRecordset->GetCollect("UserID") == m_user){
			if((CString)m_ado.m_pRecordset->GetCollect("UserPswd") == m_passw)
			{
				CString st;
				st.Format(_T("%d"),m_parent->m_Vcon.size() - 1);
				CString stri = _T("07")+ st+'\0' ;
				st = _T("00登录成功.... ") + st +'\0';
				send(m_parent->m_Vcon[m]->m_listen,StringToChar(st), st.GetLength(),0);
				m_parent->TreeStyle();
				for(int i = 1 ; i < m_parent->m_Vcon.size() ;i ++)
				{
					m_parent->m_tree.InsertItem(m_parent->m_Vcon.at(i)->m_user,NULL,NULL,m_parent->m_hItem);
				}
				m_parent->m_num.Format(_T("%d"),m_parent->m_Vcon.size() - 1);
				m_parent->UpdateData(FALSE);
				st = _T("02");
				for(int j = 1 ; j < m_parent->m_Vcon.size() ; j++)
				{
					st += _T(" ") + m_parent->m_Vcon.at(j)->m_user ;
				}
				st += '\0';
				for(int i = 1 ; i < m_parent->m_Vcon.size() ; i++)
				{
					Sleep(200);
					send(m_parent->m_Vcon[i]->m_listen,StringToChar(stri), stri.GetLength(),0);
				}
				for(int i = 1 ; i < m_parent->m_Vcon.size() ; i++)
				{
					Sleep(200);
					send(m_parent->m_Vcon[i]->m_listen,StringToChar(st), st.GetLength(),0);
				}
				return;
			}
			else
			{
				CString st = _T("00用户名与密码不匹配....")+'\0';
				send(m_parent->m_Vcon[m]->m_listen,StringToChar(st), st.GetLength(),0);
				return ;
			}
		}
		m_ado.m_pRecordset->MoveNext();
	}
	CString st = _T("00用户名不存在....")+'\0';
	send(m_parent->m_Vcon[m]->m_listen,StringToChar(st),st.GetLength(),0);
}


void CConnSocket::Register(int m)
{
	CString str;
	str.Format(_T("select UserId from Users where UserId = '%s'") , m_user);
	_bstr_t vSQL=(_bstr_t)str;
	m_ado.OnInitADOConn();
	m_ado.m_pRecordset = m_ado.GetRecordset(vSQL);
	if(m_ado.m_pRecordset->adoEOF)
	{
		str.Format(_T("insert into Users values('%s','%s')"),m_user,m_passw);
		vSQL = (_bstr_t)str;
		m_ado.ExecuteSQL(vSQL);
		CString st = _T("01注册成功....")+'\0';
		send(m_parent->m_Vcon[m]->m_listen,StringToChar(st), st.GetLength(),0);
	}
	else
	{
		CString st = _T("01用户名已存在....")+'\0';
		send(m_parent->m_Vcon[m]->m_listen,StringToChar(st), st.GetLength(),0);
	}
}


void CConnSocket::CloseClient(void)
{
	CString st;
	st.Format(_T("%d"),m_parent->m_Vcon.size() - 1);
	CString stri = _T("07")+ st+'\0' ;
	for(int i = 1 ; i < m_parent->m_Vcon.size() ; i++)
	{
		Sleep(200);
		send(m_parent->m_Vcon[i]->m_listen,StringToChar(stri), stri.GetLength(),0);
	}
	st = _T("02");
	for(int j = 1 ; j < m_parent->m_Vcon.size() ; j++)
	{
		st += _T(" ") + m_parent->m_Vcon.at(j)->m_user ;
	}
	st += '\0';
	for(int i = 1 ; i < m_parent->m_Vcon.size() ; i++)
	{
		Sleep(200);
		send(m_parent->m_Vcon[i]->m_listen,StringToChar(st), st.GetLength(),0);
	}

}
