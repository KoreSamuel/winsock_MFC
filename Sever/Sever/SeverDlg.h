
// SeverDlg.h : ͷ�ļ�
//

#pragma once
#include "ConnSocket.h"
#include "ListenSocket.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"
#include "vector"
using namespace std;

#define WM_MyMessage WM_USER+110

struct  app
{
	vector<CSocketParent*>  *m_Vcon;
	vector<WSAEVENT>  *m_VetEvent;
	char ch[4096];
	CSeverDlg * dlg;
	HWND wind;
	int len;
	int i;
};



// CSeverDlg �Ի���
class CSeverDlg : public CDialogEx
{
// ����
public:
	CSeverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SEVER_DIALOG};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListenSocket m_listenSocket;
	CTreeCtrl m_tree;
	CListBox m_list;
	CString m_information;
	app * m_app;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonPush();
	vector<CSocketParent*>  m_Vcon;
	vector<WSAEVENT>  m_VetEvent;
	CString m_num;
	HTREEITEM m_hItem;
	void TreeStyle(void);
	char *StringToChar(CString str);
	void RegistEvent(void);
	void DealEvent(void);

	void Send(char * sendData,int len,int i)
	{
		send(m_Vcon.at(i)->m_listen, sendData, len, 0);
	}
	afx_msg void OnClose();
};
