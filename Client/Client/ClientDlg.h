
// ClientDlg.h : 头文件
//

#pragma once
#include "ConnSocket.h"
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
// CClientDlg 对话框
struct  app
{
	SOCKET *socket;
	char ch[4096];
	HWND wind;
};

#define WM_MyMessage WM_USER+110

class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
