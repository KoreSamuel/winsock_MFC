
// SeverDlg.cpp : ʵ���ļ�
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
		//�������¼������ϵȴ���ֻҪ��һ���¼������Ϊ������״̬����������
		int nIndex = WSAWaitForMultipleEvents(a->m_VetEvent->size(),eventArray,false,WSA_INFINITE,false);

		//�������¼������������һ���Ǿ����������ǰ�����һ����Ȼ������ѭ��һ�δ��������¼�����
		nIndex = nIndex-WSA_WAIT_EVENT_0;
		//��ÿһ���¼�����WSAWaitForMultipleEvents�������Ա�ȷ������״̬
		for(int i = nIndex;i<a->m_VetEvent->size();i++){
			int ret;
			ret = WSAWaitForMultipleEvents(1,&eventArray[i],true,1000,false);
			if(ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
				continue;
			else{
				//��ȡ����֪ͨ��Ϣ��WSAEnumNetworkEvents�����Զ����������¼�
				WSANETWORKEVENTS wsaEvent;
				WSAEnumNetworkEvents(a->m_Vcon->at(i)->m_listen,eventArray[i],&wsaEvent);
				//����FD_ACCEPT֪ͨ��Ϣ
				if(wsaEvent.lNetworkEvents & FD_ACCEPT){
					//�������FD_ACCEPT��Ϣʱû�д���
					if(wsaEvent.iErrorCode[FD_ACCEPT_BIT] == 0){
						//������̫�࣬��ʱ������
						if(a->m_Vcon->size() > 1000){
							continue;
						}


						//�����������󣬵õ���ͻ��˽���ͨ�ŵ�Socket��Event
						SOCKET sNew = accept(a->m_Vcon->at(i)->m_listen,NULL,NULL);

						WSAEVENT newEvent = WSACreateEvent();
						//���½����¼�����newEvent������Socket��
						//ע��FD_READ|FD_CLOSE|FD_WRITE�����¼�
						WSAEventSelect(sNew,newEvent,FD_READ|FD_CLOSE|FD_WRITE);
						a->m_VetEvent->push_back(newEvent);
						CConnSocket * conn = new CConnSocket();
						conn->m_listen = sNew;
						conn->m_parent = a->dlg;
						a->m_Vcon->push_back(conn);
					}
				}else if(wsaEvent.lNetworkEvents & FD_READ){
					if(wsaEvent.iErrorCode[FD_READ_BIT] == 0){
						//������Ϣ
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


// CSeverDlg �Ի���




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


// CSeverDlg ��Ϣ�������

BOOL CSeverDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	TreeStyle();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSeverDlg::OnPaint()
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	m_hItem = m_tree.InsertItem(_T("����������Ա"),NULL,NULL);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnBnClickedButtonStop();
	CDialogEx::OnClose();
}
