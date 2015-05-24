
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "ConnSocket.h"
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
// CClientDlg �Ի���
struct  app
{
	SOCKET *socket;
	char ch[4096];
	HWND wind;
};

#define WM_MyMessage WM_USER+110

class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

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
	~CClientDlg();
	afx_msg void OnBnClickedButtonClose();
	int m_port;
	CString m_information;
	CConnSocket m_ConnSocket;
	CListBox m_list;
	DWORD m_ip;
	afx_msg void OnBnClickedButtonConn();
	afx_msg void OnLbnSelchangeListDlg();
	CString m_show_user;
	CString m_login_user;
	app * m_app;
	CString m_login_passw;
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonRegister();
	afx_msg void OnBnClickedButtonCheat();
	CComboBox m_combo;
	CString m_combo_value;
	CIPAddressCtrl m_ipControl;
	afx_msg void OnBnClickedButtonGroup();
	char * StringToChar(CString str);
};
