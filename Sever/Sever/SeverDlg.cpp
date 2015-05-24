
// SeverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Sever.h"
#include "SeverDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI IOThread(LPVOID lpParam)
{
	app *a = (app *) lpParam;
	while(true){
		WSAEVENT eventArray[1000];
		for(int i = 0 ; i < a->m_VetEvent->size() ; i++)
			eventArray[i] = a->m_VetEvent->at(i);
		//在所有事件对象上等待，只要有一个事件对象变为已授信状态，则函数返回
		int nIndex = WSAWaitForMultipleEvents(a->m_VetEvent->size(),eventArray,false,WSA_INFINITE,false);

		//发生的事件对象的索引，一般是句柄数组中最前面的那一个，然后再用循环一次处理后面的事件对象
		nIndex = nIndex-WSA_WAIT_EVENT_0;
		//对每一个事件调用WSAWaitForMultipleEvents函数，以便确定它的状态
		for(int i = nIndex;i<a->m_VetEvent->size();i++){
			int ret;
			ret = WSAWaitForMultipleEvents(1,&eventArray[i],true,1000,false);
			if(ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
				continue;
			else{
				//获取来的通知信息，WSAEnumNetworkEvents函数自动重置授信事件
				WSANETWORKEVENTS wsaEvent;
				WSAEnumNetworkEvents(a->m_Vcon->at(i)->m_listen,eventArray[i],&wsaEvent);
				//处理FD_ACCEPT通知消息
				if(wsaEvent.lNetworkEvents & FD_ACCEPT){
					//如果处理FD_ACCEPT消息时没有错误
					if(wsaEvent.iErrorCode[FD_ACCEPT_BIT] == 0){
						//连接数太多，暂时不处理
						if(a->m_Vcon->size() > 1000){
							continue;
						}


						//接收连接请求，得到与客户端进行通信的Socket和Event
						SOCKET sNew = accept(a->m_Vcon->at(i)->m_listen,NULL,NULL);

						WSAEVENT newEvent = WSACreateEvent();
						//将新建的事件对象newEvent关联到Socket上
						//注册FD_READ|FD_CLOSE|FD_WRITE网络事件
						WSAEventSelect(sNew,newEvent,FD_READ|FD_CLOSE|FD_WRITE);
						a->m_VetEvent->push_back(newEvent);
						CConnSocket * conn = new CConnSocket();
						conn->m_listen = sNew;
						conn->m_parent = a->dlg;
						a->m_Vcon->push_back(conn);
					}
				}else if(wsaEvent.lNetworkEvents & FD_READ){
					if(wsaEvent.iErrorCode[FD_READ_BIT] == 0){
						//接收消息
						memset(a->ch,'\0',sizeof(a->ch));
						int nRecv = recv(a->m_Vcon->at(i)->m_listen,a->ch,4096,0);
						a->ch[nRecv] = '\0';
						a->len = nRecv;
						a->i = i;
						if(nRecv > 0){
							SendMessage(a->wind,WM_MyMessage,0,(LPARAM)a);
						}else{
							strcpy(a->ch , "08"+'\0');
							a->len = 2;
							a->i = i;
							SendMessage(a->wind,WM_MyMessage,0,(LPARAM)a);
						}
					}
				}
			}
			Sleep(0);
		}
	}
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


// CSeverDlg 对话框




CSeverDlg::CSeverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSeverDlg::IDD, pParent)
	, m_information(_T(""))
	, m_num(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_listenSocket.SetParent(this);
	m_app = new app;
}

void CSeverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_INFOR, m_information);
	DDX_Text(pDX, IDC_EDIT1, m_num);
}

BEGIN_MESSAGE_MAP(CSeverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MyMessage, &CSeverDlg::OnMyMessage)
	ON_BN_CLICKED(IDC_BUTTON_START, &CSeverDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSeverDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PUSH, &CSeverDlg::OnBnClickedButtonPush)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSeverDlg 消息处理程序

BOOL CSeverDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	TreeStyle();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

LRESULT CSeverDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	strcpy(m_Vcon.at(m_app->i)->m_szBuffer , m_app->ch);
	m_Vcon.at(m_app->i)->Receive(m_app->len ,m_app->i);
	return 0;
}

void CSeverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSeverDlg::OnPaint()
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
HCURSOR CSeverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSeverDlg::OnBnClickedButtonStart()
{
	m_listenSocket.Init();
	if(m_listenSocket.m_listen!=INVALID_SOCKET)
	{
		if(m_listenSocket.Create(4000) == SOCKET_ERROR)
			return ;
		m_listenSocket.Listen(5);
		RegistEvent();

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);

		m_app->m_Vcon = &m_Vcon;
		m_app->m_VetEvent = &m_VetEvent;
		m_app->dlg = this;
		m_app->wind = this->GetSafeHwnd();
		CreateThread(NULL,NULL,IOThread,m_app,0,NULL);
	}
}

void CSeverDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i = 1 ;i < m_Vcon.size() ; i++)
	{
		CString st = _T("10")+'\0';
		Send(StringToChar(st),st.GetLength(),i);
		m_Vcon.at(i)->Close();
	}

	TreeStyle();
	m_num = _T("0");
	if(m_Vcon.size() == 0)
		return ;
	m_Vcon.at(0)->Close();
	m_Vcon.clear();
	m_VetEvent.clear();
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);
	UpdateData(FALSE);
}


void CSeverDlg::OnBnClickedButtonPush()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	CString str = _T("server:");
	if(!m_information.IsEmpty()){
		str += m_information;
		for(int i = 0 ;i < m_Vcon.size() ;i ++)
		{
			Send(StringToChar(str), str.GetLength(),i);
		}
		m_list.AddString(str);
		m_information.Empty();
		UpdateData(FALSE);
	}
}


void CSeverDlg::TreeStyle(void)
{
	m_tree.DeleteAllItems();
	m_tree.ModifyStyle(NULL,TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
	m_hItem = m_tree.InsertItem(_T("所有在线人员"),NULL,NULL);
}

char * CSeverDlg::StringToChar(CString str)
{
	return (LPSTR)(LPCTSTR)(str);
}


void CSeverDlg::RegistEvent(void)
{
	WSAEVENT event = WSACreateEvent();
	WSAEventSelect(m_listenSocket.m_listen,event,FD_ACCEPT|FD_CLOSE);

	m_VetEvent.push_back(event);
	m_Vcon.push_back(&m_listenSocket);
}




void CSeverDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedButtonStop();
	CDialogEx::OnClose();
}
