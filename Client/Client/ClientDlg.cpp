
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "string.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


 DWORD WINAPI IOThread(LPVOID lpParam)
 {
	app *host = (app*) lpParam;
 	while(true){
 		memset(host->ch,0,4096);
 		int retVal = recv(*host->socket, host->ch, 4096, 0);
 		SendMessage(host->wind,WM_MyMessage,0,retVal);
 	}
 	return 0;
 }

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg 对话框




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
	, m_port(0)
	, m_information(_T(""))
	, m_ip(0)
	, m_show_user(_T(""))
	, m_login_user(_T(""))
	, m_login_passw(_T(""))
	, m_combo_value(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ConnSocket.SetParent(this);
	m_app = new app;
}
CClientDlg::~CClientDlg(){
	CString str = _T("08")+m_login_user+'\0';
	m_ConnSocket.Send(StringToChar(str),str.GetLength());
	m_ConnSocket.Close();
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_INFOR, m_information);
	DDX_Control(pDX, IDC_LIST_DLG, m_list);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_ip);
	DDX_Text(pDX, IDC_EDIT_SUSER, m_show_user);
	DDX_Text(pDX, IDC_EDIT_LUSER, m_login_user);
	DDX_Text(pDX, IDC_EDIT_PASSW, m_login_passw);
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_CBString(pDX, IDC_COMBO, m_combo_value);
	DDX_Control(pDX, IDC_IPADDRESS, m_ipControl);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MyMessage, &CClientDlg::OnMyMessage)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CClientDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CONN, &CClientDlg::OnBnClickedButtonConn)
	ON_LBN_SELCHANGE(IDC_LIST_DLG, &CClientDlg::OnLbnSelchangeListDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CClientDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CClientDlg::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_CHEAT, &CClientDlg::OnBnClickedButtonCheat)
	ON_BN_CLICKED(IDC_BUTTON_Group, &CClientDlg::OnBnClickedButtonGroup)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CHEAT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CONN)->EnableWindow(true);
	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

LRESULT CClientDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	strcpy(m_ConnSocket.m_szBuffer , m_app->ch);
	m_ConnSocket.Receive(lParam);
    return 0;
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = _T("08") + m_login_user + '\0';
	m_ConnSocket.Send(StringToChar(str),str.GetLength());
	m_ConnSocket.Close();
	m_list.ResetContent();
	m_login_passw = _T("");
	m_login_user = _T("");
	m_show_user = _T("");
	m_port = 0;
	m_ip = 0;
	m_combo.ResetContent();
	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_CONN)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_CHEAT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(false);
}


void CClientDlg::OnBnClickedButtonConn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_ConnSocket.Create() != INVALID_SOCKET){
		BYTE a,b,c,d;
		m_ipControl.GetAddress(a,b,c,d);
		CString str; 
		str.Format(_T("%d.%d.%d.%d"),127 ,0,0,1); 

		if(m_ConnSocket.Connect(StringToChar(str),4000) == SOCKET_ERROR)
		{
			MessageBox(_T("连接服务器失败"));
			return ;
		}
		GetDlgItem(IDC_BUTTON_CONN)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_CHEAT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(true);

		m_app->socket = &m_ConnSocket.m_host;
		m_app->wind = this->GetSafeHwnd();
		CreateThread(NULL,NULL,IOThread,m_app,0,NULL);
	}
	else
	{
		MessageBox(_T("Socket创建失败！"));
	}	
}


void CClientDlg::OnLbnSelchangeListDlg()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString str = _T("");
	if(m_login_user != "" && m_login_passw != "")
	{
		str = _T("00") + m_login_user + _T(" ") + m_login_passw;
		m_ConnSocket.Send(StringToChar(str), str.GetLength());
	}
	else
	{
		AfxMessageBox(_T("请把数据填写完整..."));
	}
}


void CClientDlg::OnBnClickedButtonRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString str = _T("");
	if(m_login_user != "" && m_login_passw != "")
	{
		str = _T("01") + m_login_user + _T(" ") + m_login_passw+'\0';
		m_ConnSocket.Send(StringToChar(str), str.GetLength());
	}
	else
	{
		AfxMessageBox(_T("请把数据填写完整..."));
	}
}


void CClientDlg::OnBnClickedButtonCheat()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString str = m_login_user + _T(":");
	if(!m_information.IsEmpty()){
		str += m_information;
		CString s = _T("03")+ m_combo_value +" "+ str;
		m_ConnSocket.Send(StringToChar(s), s.GetLength());
		if(m_combo_value != "")
			m_list.AddString(str);
		m_information.Empty();
		UpdateData(FALSE);
	}
}


void CClientDlg::OnBnClickedButtonGroup()
{
	UpdateData(true);
	CString str = m_login_user + _T(":");
	if(!m_information.IsEmpty()){
		str += m_information;
		CString s = _T("03 ")+ str;
		m_ConnSocket.Send(StringToChar(s), s.GetLength());
		m_information.Empty();
		UpdateData(FALSE);
	}
}

char * CClientDlg::StringToChar(CString str)
{
	return (LPSTR)(LPCTSTR)(str);
}

