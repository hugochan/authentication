
// authentication_clientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "authentication_client.h"
#include "authentication_clientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UM_SOCK WM_USER + 100
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// Cauthentication_clientDlg dialog



Cauthentication_clientDlg::Cauthentication_clientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cauthentication_clientDlg::IDD, pParent)
	, password(12345)
	, RemotePort(8000)
	, SendMsgText(_T(""))
	, DisplayText(_T(""))
{
	//DWORD defaultIP = inet_addr(CString("127.0.0.1"));
	//RemoteIP.SetAddress((BYTE)127, (BYTE)0, (BYTE)0, (BYTE)1);
	//STATE current_state = STATE4;
	current_state = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cauthentication_clientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SETPASSWORD, password);
	//  DDX_IPAddress(pDX, IDC_IPADDRESS_SETREMOTEIP, RemoteIP);
	DDX_Text(pDX, IDC_EDIT_SETREMOTEPORT, RemotePort);
	DDX_Text(pDX, IDC_EDIT_SENDMSG, SendMsgText);
	DDX_Text(pDX, IDC_EDIT_DLGDISPLAY, DisplayText);
	DDX_Control(pDX, IDC_IPADDRESS_SETREMOTEIP, RemoteIP);
}

BEGIN_MESSAGE_MAP(Cauthentication_clientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_STARTCONNECT, &Cauthentication_clientDlg::OnBnClickedButtonStartconnect)
	ON_BN_CLICKED(IDC_BUTTON_STOPCONNECT, &Cauthentication_clientDlg::OnBnClickedButtonStopconnect)
	ON_BN_CLICKED(IDC_BUTTON_SENDMSG, &Cauthentication_clientDlg::OnBnClickedButtonSendmsg)
END_MESSAGE_MAP()


// Cauthentication_clientDlg message handlers

BOOL Cauthentication_clientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cauthentication_clientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cauthentication_clientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cauthentication_clientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cauthentication_clientDlg::OnBnClickedButtonStartconnect()
{
	if (0 == current_state){
		int retval;
		sockaddr_in server;
		WSAData wsa;
		UpdateData(TRUE);

		WSAStartup(0x101, &wsa);


		sock = socket(AF_INET, SOCK_STREAM, 0);

		if (RemotePort > 65535){//检查端口范围
			MessageBox(CString("port number error!"), CString("client"), MB_OK);
			return;
		}

		server.sin_family = AF_INET;
		RemoteIP.GetAddress(server.sin_addr.S_un.S_un_b.s_b1, server.sin_addr.S_un.S_un_b.s_b2, server.sin_addr.S_un.S_un_b.s_b3, server.sin_addr.S_un.S_un_b.s_b4);
		server.sin_port = htons(RemotePort);

		WSAAsyncSelect(sock, m_hWnd, UM_SOCK, FD_CLOSE | FD_WRITE);
		retval = connect(sock, (sockaddr*)&server, sizeof(server));
		if (retval == -1){
			retval = WSAGetLastError();
			if (retval != WSAEWOULDBLOCK){
				closesocket(sock);
				sock = 0;
				WSACleanup();
				DisplayText = _T("");
				UpdateData(FALSE);
				MessageBox(CString("connect error!"), CString("client"), MB_OK);
			}
			else{
				MessageBox(CString("connect success!"), CString("client"), MB_OK);
			}
		}
		else{
			MessageBox(CString("connect success!"), CString("client"), MB_OK);
		}
	}
	else{
		MessageBox(CString("connection has been established!"), CString("client"), MB_OK);
	}
}


void Cauthentication_clientDlg::OnBnClickedButtonStopconnect()
{
	if (current_state != 0){
		closesocket(sock);
		sock = 0;
		current_state = 0;
		WSACleanup();
		DisplayText = _T("");
		UpdateData(FALSE);
		MessageBox(CString("stop the connection!"), CString("client"), MB_OK);
	}
	else{
		MessageBox(CString("You haven`t started the connection!"), CString("client"), MB_OK);
	}
}


void Cauthentication_clientDlg::OnBnClickedButtonSendmsg()
{
	if (3 == current_state){//判断是否为工作状态
		UpdateData(TRUE);
		int len, retval;
		CString tempBuf;
		len = SendMsgText.GetLength();
		if (0 != len){
			data_to_server.Type = type_chat_DataToServer;
			data_to_server.content = SendMsgText;
			retval = send(sock, (char*)&data_to_server, sizeof(data_to_server), 0);
			retval = send(sock, (char*)SendMsgText.GetBuffer(0), SendMsgText.GetLength(), 0);
			SendMsgText.ReleaseBuffer();
			if (retval == -1){
				retval = WSAGetLastError();
				if (retval != WSAEWOULDBLOCK){
					closesocket(sock);
					current_state = 0;
					sock = 0;
					WSACleanup();
					DisplayText = _T("");
					UpdateData(FALSE);
					MessageBox(CString("send error! closed connection!"), CString("client"), MB_OK);
				}
			}
			else{
				tempBuf.Format(_T("%s"), "我： ");
				DisplayText += (tempBuf + SendMsgText + CString("\r\n"));
				SendMsgText = _T("");
				UpdateData(FALSE);
			}
		}
		else{
			MessageBox(CString("There is no content to send!"), CString("client"), MB_OK);
		}
	}
	else if (0 == current_state){
		MessageBox(CString("no connection to server!"), CString("client"), MB_OK);
	}
	else{
		MessageBox(CString("can`t send now!"), CString("client"), MB_OK);
	}
}

LRESULT Cauthentication_clientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int Event;
	int len, retval, i;
	USHORT lineNum, countPerLine = 25;
	char buf[4096];
	CString tempBuf("");
	UINT queryResult;
	switch (message){
	case UM_SOCK:
		Event = LOWORD(lParam);
		switch (Event){
		case FD_CONNECT:
			break;
		case FD_WRITE:
			current_state = 1;//连接上服务器，进入下一状态
			MessageBox(CString("connect to server!"), CString("client"), MB_OK);
			WSAAsyncSelect(sock, m_hWnd, UM_SOCK, FD_READ | FD_CLOSE);
			break;
		case FD_READ:
			len = recv(sock, (char *)&data_from_server, sizeof(data_from_server), 0);
			if (len <= 0){
				retval = WSAGetLastError();
				if (retval != WSAEWOULDBLOCK){
					closesocket(sock);//关闭套接字
					current_state = 0;
					DisplayText = _T("");
					UpdateData(FALSE);
					MessageBox(CString("failed to recv! closed connection!"), CString("client"), MB_OK);
					break;
				}
				else
					break;
			}
			else{
				if (3 == current_state){//判断是否为工作状态
					if (type_chat_DataFromServer == data_from_server.Type){
						len = recv(sock, buf, strlen(buf), 0);//????
						if (len <= 0){
							retval = WSAGetLastError();
							if (retval != WSAEWOULDBLOCK){
								closesocket(sock);//关闭套接字
								current_state = 0;
								DisplayText = _T("");
								UpdateData(FALSE);
								MessageBox(CString("failed to recv! closed connection!"), CString("client"), MB_OK);
								break;
							}
							else
								break;
						}
						else{
							buf[len] = 0;
							tempBuf.Format(_T("%s"), buf);
							//data_from_server.content = tempBuf;
							DisplayText += (CString("server： ") + tempBuf + CString("\r\n"));
							//arrange the text to make it well organized
							/*			len = tempBuf.GetLength();
							if (len % countPerLine == 0){
							lineNum = len / countPerLine;
							}
							else lineNum = len / countPerLine + 1;
							for (i = 0; i < lineNum - 1; i++){
							DisplayText += (tempBuf.Mid(i*countPerLine, countPerLine) + CString("\r\n"));
							}
							DisplayText += tempBuf.Right(len - (lineNum - 1)*countPerLine) + CString("\r\n\r\n");
							*/
							UpdateData(FALSE);
							break;
						}
					}
					else{
						break;
					}
				}
				else if (2 == current_state){//等待验证结果状态
					if (type_reply == data_from_server.Type){
						len = recv(sock, buf, strlen(buf), 0);
						if (len <= 0){
							retval = WSAGetLastError();
							if (retval != WSAEWOULDBLOCK){
								closesocket(sock);//关闭套接字
								current_state = 0;
								DisplayText = _T("");
								UpdateData(FALSE);
								MessageBox(CString("failed to recv! closed connection!"), CString("client"), MB_OK);
								break;
							}
							else
								break;
						}
						else{
							buf[len] = 0;
							tempBuf.Format(_T("%s"), buf);
							//data_from_server.content = tempBuf;
							if (valid == tempBuf){
								current_state = 3;
								MessageBox(CString("通过验证!"), CString("client"), MB_OK);
								break;
							}
							else if(invalid == data_from_server.content){
								closesocket(sock);//关闭套接字
								sock = 0;
								WSACleanup();
								current_state = 0;
								DisplayText = _T("");
								UpdateData(FALSE);
								MessageBox(CString("验证失败!"), CString("client"), MB_OK);
								break;
							}
							else{
								break;
							}
						}
					}
					else{
						break;
					}
				}
				else if (1 == current_state){//等待质询数据状态
					if (type_query == data_from_server.Type){
						data_from_server.data_2byte = new USHORT[(const USHORT)data_from_server.count_2byte];
						data_from_server.data_4byte = new UINT[(const USHORT)data_from_server.count_4byte];

						len = recv(sock, (char *)data_from_server.data_2byte, sizeof(USHORT) * data_from_server.count_2byte, 0);
						len = recv(sock, (char *)data_from_server.data_4byte, sizeof(UINT) * data_from_server.count_4byte, 0);
						delete data_from_server.data_2byte;
						delete data_from_server.data_4byte;
						if (len <= 0){
							retval = WSAGetLastError();
							if (retval != WSAEWOULDBLOCK){
								closesocket(sock);//关闭套接字
								current_state = 0;
								DisplayText = _T("");
								UpdateData(FALSE);
								MessageBox(CString("failed to recv! closed connection!"), CString("client"), MB_OK);
								break;
							}
							else
								break;
						}
						else{
							//计算质询结果
							if (FALSE){
							
							}
							else{
								data_to_server.Type = type_result;
								data_to_server.queryResult = 2048;
								retval = send(sock, (char*)&data_to_server, sizeof(data_to_server), 0);
								if (retval == -1){
									retval = WSAGetLastError();
									if (retval != WSAEWOULDBLOCK){
										closesocket(sock);
										current_state = 0;
										sock = 0;
										WSACleanup();
										DisplayText = _T("");
										UpdateData(FALSE);
										MessageBox(CString("send query result error! closed connection!"), CString("client"), MB_OK);
										break;
									}
								}
								else{
									current_state = 2;
								}
							}
						}
					}
					else{
						break;
					}
				}
				else{
				//To do
				}
			}
			break;
		case FD_CLOSE://????
			closesocket(sock);
			sock = 0;
			WSACleanup();
			current_state = 0;
			DisplayText = _T("");
			UpdateData(FALSE);
			break;
		}
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}
