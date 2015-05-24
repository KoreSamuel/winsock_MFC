
// ClientDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CClientDlg �Ի���




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


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CHEAT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CONN)->EnableWindow(true);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if(m_ConnSocket.Create() != INVALID_SOCKET){
		BYTE a,b,c,d;
		m_ipControl.GetAddress(a,b,c,d);
		CString str; 
		str.Format(_T("%d.%d.%d.%d"),127 ,0,0,1); 

		if(m_ConnSocket.Connect(StringToChar(str),4000) == SOCKET_ERROR)
		{
			MessageBox(_T("���ӷ�����ʧ��"));
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
		MessageBox(_T("Socket����ʧ�ܣ�"));
	}	
}


void CClientDlg::OnLbnSelchangeListDlg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CString str = _T("");
	if(m_login_user != "" && m_login_passw != "")
	{
		str = _T("00") + m_login_user + _T(" ") + m_login_passw;
		m_ConnSocket.Send(StringToChar(str), str.GetLength());
	}
	else
	{
		AfxMessageBox(_T("���������д����..."));
	}
}


void CClientDlg::OnBnClickedButtonRegister()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CString str = _T("");
	if(m_login_user != "" && m_login_passw != "")
	{
		str = _T("01") + m_login_user + _T(" ") + m_login_passw+'\0';
		m_ConnSocket.Send(StringToChar(str), str.GetLength());
	}
	else
	{
		AfxMessageBox(_T("���������д����..."));
	}
}


void CClientDlg::OnBnClickedButtonCheat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

