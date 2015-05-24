#include "StdAfx.h"
#include "ConnSocket.h"
#include "ClientDlg.h"


CConnSocket::CConnSocket(void)
{

}


CConnSocket::~CConnSocket(void)
{
}

int CConnSocket::Connect(char *ipAddress,int port)
{
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr(ipAddress);  // �û���Ҫ����ʵ������޸�
	servAddr.sin_port = htons(port);             // ��ʵ��Ӧ���У����齫��������IP��ַ�Ͷ˿ںű����������ļ���
	int sServerAddlen = sizeof(servAddr);            // �����ַ�ĳ���       
	// ���ӷ�����   
	return connect(m_host, (LPSOCKADDR)&servAddr, sizeof(servAddr));
}


void CConnSocket::SetParent(CClientDlg * parent)
{
	m_parent = parent;
}


void CConnSocket::Receive(int num)
{
	static bool success = false;
	// TODO: �ڴ����ר�ô����/����û���
	CString str = _T("");
	if(num != SOCKET_ERROR){
		str.Empty();
		m_szBuffer[num] = '\0';
		str = m_szBuffer;
		if(str.Left(2) == "00" || str.Left(2) == "01")//��¼
		{ 
			int index;
			if((index = str.Find(_T(" "))) != -1)
			{
				if(str.Mid(2,index - 2).Find("��½�ɹ�"))
					success = true;
				else{
				   AfxMessageBox(str.Mid(2,index - 2));
				   success = false;
				}
				m_parent->m_show_user = str.Mid(index + 1 , str.GetLength() - 1);
				m_parent->GetDlgItem(IDC_BUTTON_CONN)->EnableWindow(false);
				m_parent->GetDlgItem(IDC_BUTTON_CHEAT)->EnableWindow(true);
				m_parent->GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(true);
				m_parent->GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(true);
				m_parent->GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(true);
				m_parent->UpdateData(false);
			}
			else
			{
				AfxMessageBox(str.Mid(2,str.GetLength()-1));
			}
		}
		else if(str.Left(2) == "02")
		{
			str = str.Mid(2,str.GetLength()-1);
			int nItem = 0;
			m_parent->m_combo.ResetContent();

			CString resToken;
			int curPos = 0;
			resToken= str.Tokenize(_T(" "),curPos);
			while (resToken != _T(""))
			{
				m_parent->m_combo.AddString(resToken);
				
				resToken= str.Tokenize(_T(" "),curPos);
			}
			if(success){
				AfxMessageBox(_T("��½�ɹ�"));
				success = false;
			}
			m_parent->GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(false);
			m_parent->UpdateData(FALSE);
		}
		else if(str.Left(2) == "04" || str.Left(2) == "05")
		{
			m_parent->m_list.AddString(str.Mid(2,str.GetLength()-1));
			m_parent->UpdateData(false);
		}
		else if(str.Left(2) == "07")
		{
			m_parent->m_show_user = str.Mid(2,str.GetLength()-1);
			m_parent->UpdateData(FALSE);
		}
		else if(str.Left(2) == "10")
		{
			CConnSocket::Close();
			m_parent->GetDlgItem(IDC_BUTTON_CONN)->EnableWindow(true);
			m_parent->GetDlgItem(IDC_BUTTON_CHEAT)->EnableWindow(false);
			m_parent->GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(false);
			m_parent->GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(false);
			m_parent->GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(false);
			m_parent->GetDlgItem(IDC_BUTTON_Group)->EnableWindow(false);
			m_parent->m_list.ResetContent();
			m_parent->m_login_passw = _T("");
			m_parent->m_login_user = _T("");
			m_parent->m_show_user = _T("");
			m_parent->m_port = 0;
			m_parent->m_ip = 0;
			m_parent->m_combo.ResetContent();
			m_parent->UpdateData(false);
		}
		else
		{
			m_parent->m_list.AddString(str);
			m_parent->UpdateData(FALSE);
		}
	}
}

void CConnSocket::Send(char* sendData,int len)
{
	send(m_host, sendData, len, 0);
}
