
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
#define UM_SOCK_TRANS WM_USER + 101
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
	, clientName(_T(""))
	, password(12345)
	, RemotePort(8000)
	, TransPort(9000)
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
	DDX_Text(pDX, IDC_EDIT_CLIENTNAME, clientName);
}

BEGIN_MESSAGE_MAP(Cauthentication_clientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_STARTCONNECT, &Cauthentication_clientDlg::OnBnClickedButtonStartconnect)
	ON_BN_CLICKED(IDC_BUTTON_STOPCONNECT, &Cauthentication_clientDlg::OnBnClickedButtonStopconnect)
	ON_BN_CLICKED(IDC_BUTTON_SENDMSG, &Cauthentication_clientDlg::OnBnClickedButtonSendmsg)
	ON_BN_CLICKED(IDC_BUTTON_UPLOADFILE, &Cauthentication_clientDlg::OnBnClickedButtonUploadfile)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOADFILE, &Cauthentication_clientDlg::OnBnClickedButtonDownloadfile)
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


		main_sock = socket(AF_INET, SOCK_STREAM, 0);//主套接字

		if (RemotePort > 65535){//检查端口范围
			MessageBox(CString("port number error!"), CString("client"), MB_OK);
			return;
		}
		if (clientName == _T("")){//检查用户名
			MessageBox(CString("user name can`t be empty!"), CString("client"), MB_OK);
			return;
		}
		if (password == NULL){
			MessageBox(CString("password can`t be empty!"), CString("client"), MB_OK);
			return;
		}

		server.sin_family = AF_INET;
		RemoteIP.GetAddress(server.sin_addr.S_un.S_un_b.s_b1, server.sin_addr.S_un.S_un_b.s_b2, server.sin_addr.S_un.S_un_b.s_b3, server.sin_addr.S_un.S_un_b.s_b4);
		server.sin_port = htons(RemotePort);

		WSAAsyncSelect(main_sock, m_hWnd, UM_SOCK, FD_CLOSE | FD_WRITE);

		retval = connect(main_sock, (sockaddr*)&server, sizeof(server));
		if (retval == -1){
			retval = WSAGetLastError();
			if (retval != WSAEWOULDBLOCK){
				closesocket(main_sock);
				main_sock = 0;
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
		closesocket(main_sock);
		main_sock = 0;
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
			retval = send(main_sock, (char*)&data_to_server, sizeof(data_to_server), 0);
			retval = send(main_sock, (char*)SendMsgText.GetBuffer(0), SendMsgText.GetLength(), 0);
			SendMsgText.ReleaseBuffer();
			if (retval == -1){
				retval = WSAGetLastError();
				if (retval != WSAEWOULDBLOCK){
					closesocket(main_sock);
					current_state = 0;
					main_sock = 0;
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
	const UINT bufSize = 4096;
	char buf[bufSize];
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
			WSAAsyncSelect(main_sock, m_hWnd, UM_SOCK, FD_READ | FD_CLOSE);
			break;
		case FD_READ:
			len = recv(main_sock, (char *)&data_from_server, sizeof(data_from_server), 0);
			if (len <= 0){
				retval = WSAGetLastError();
				if (retval != WSAEWOULDBLOCK){
					closesocket(main_sock);//关闭套接字
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
				switch (current_state){//判断是否为工作状态
				case 3:
					switch (data_from_server.Type){
						case type_chat_DataFromServer:
							len = recv(main_sock, buf, strlen(buf), 0);//????
							if (len <= 0){
								retval = WSAGetLastError();
								if (retval != WSAEWOULDBLOCK){
									closesocket(main_sock);//关闭套接字
									current_state = 0;
									DisplayText = _T("");
									UpdateData(FALSE);
									MessageBox(CString("failed to recv! closed connection!"), CString("client"), MB_OK);
								}
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
							}
							break;
						case type_upload_DataFromServer:
							len = recv(main_sock, (char *&)file_abstract_recv, sizeof(file_abstract_recv), 0);
							if (len <= 0){
								retval = WSAGetLastError();
								if (retval != WSAEWOULDBLOCK){
									closesocket(main_sock);//关闭套接字
									current_state = 0;
									MessageBox(CString("failed to recv! closed connection!"), CString("client"), MB_OK);
								}
							}
							else{
								MessageBox(CString("want to recv a file?") + file_abstract_recv.filepath, CString("client"), MB_OK);
								//To do?????
								if (tran_sock.current_state == 0){
									if(startTransConnect() == -1){
										return 0;//文件传输连接建立失败
									}
									else{
										file_trans_head.transType = transType_download;
										retval = send(tran_sock.transock, (char *)&file_trans_head, sizeof(file_trans_head), 0);
										if (retval == -1){
											retval = WSAGetLastError();
											if (retval != WSAEWOULDBLOCK){
												closesocket(tran_sock.transock);
											}
										}
									}
								}
									
							}
							break;
					}
					break;
				case 2://等待验证结果状态
					if (type_reply == data_from_server.Type){
						len = recv(main_sock, buf, strlen(buf), 0);
						if (len <= 0){
							retval = WSAGetLastError();
							if (retval != WSAEWOULDBLOCK){
								closesocket(main_sock);//关闭套接字
								current_state = 0;
								DisplayText = _T("");
								UpdateData(FALSE);
								MessageBox(CString("failed to recv! closed connection!"), CString("client"), MB_OK);
							}
						}
						else{
							buf[len] = 0;
							tempBuf.Format(_T("%s"), buf);
							//data_from_server.content = tempBuf;
							if (valid == tempBuf){
								current_state = 3;
								MessageBox(CString("通过验证!"), CString("client"), MB_OK);
							}
							else if(invalid == data_from_server.content){
								closesocket(main_sock);//关闭套接字
								main_sock = 0;
								WSACleanup();
								current_state = 0;
								DisplayText = _T("");
								UpdateData(FALSE);
								MessageBox(CString("验证失败!"), CString("client"), MB_OK);
							}
						}
					}
					break;
				case 1://等待质询数据状态
					if (type_query == data_from_server.Type){
						data_from_server.data_2byte = new USHORT[(const USHORT)data_from_server.count_2byte];
						data_from_server.data_4byte = new UINT[(const USHORT)data_from_server.count_4byte];

						len = recv(main_sock, (char *)data_from_server.data_2byte, sizeof(USHORT) * data_from_server.count_2byte, 0);
						len = recv(main_sock, (char *)data_from_server.data_4byte, sizeof(UINT) * data_from_server.count_4byte, 0);
						delete data_from_server.data_2byte;
						delete data_from_server.data_4byte;
						if (len <= 0){
							retval = WSAGetLastError();
							if (retval != WSAEWOULDBLOCK){
								closesocket(main_sock);//关闭套接字
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
								char *tempName = (LPSTR)(LPCTSTR)clientName;
								i = 0;
								while (*(tempName + i)){
									if (i < maxNumOfClientName){
										data_to_server.clientName[i] = *(tempName + i);
										file_trans_head.clientName[i] = *(tempName + i);
									}
									i++;
								}
								data_to_server.clientName[i] = '\0';
								data_to_server.queryResult = 2048;
								retval = send(main_sock, (char*)&data_to_server, sizeof(data_to_server), 0);
								if (retval == -1){
									retval = WSAGetLastError();
									if (retval != WSAEWOULDBLOCK){
										closesocket(main_sock);
										current_state = 0;
										main_sock = 0;
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
				default:
					break;
				}
			}
			break;
		case FD_CLOSE://????
			main_sock = 0;
			current_state = 0;
			//关闭文件传输套接字
			closesocket(tran_sock.transock);

			WSACleanup();
			DisplayText = _T("");
			UpdateData(FALSE);
			break;
		}
		break;
	case UM_SOCK_TRANS:
		Event = LOWORD(lParam);
		switch (Event){
		case FD_WRITE:
			tran_sock.current_state = 1;//服务成功开启
			WSAAsyncSelect(tran_sock.transock, m_hWnd, UM_SOCK_TRANS, FD_CLOSE | FD_READ);
			break;
		case FD_READ:

			len = recv(tran_sock.transock, buf, bufSize, 0);
			if (len <= 0){
				retval = WSAGetLastError();
				if (retval != WSAEWOULDBLOCK){
					closesocket(tran_sock.transock);//关闭文件传输套接字
					//MessageBox(CString("failed to recv file!"), CString("server"), MB_OK);
				}
			}
			else{
				if (len < bufSize){
					buf[len] = '\0';
				}
				DWORD dwWrite;
				bool bRet;
				//保存文件
				HANDLE hFile = CreateFile(file_abstract_recv.filepath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE) return 0;
				SetFilePointer(hFile, 0, NULL, FILE_END);
				bRet = WriteFile(hFile, buf, len, &dwWrite, NULL);
				if (bRet == FALSE){
					MessageBox(CString("write file error!"), CString("server"), MB_OK);

				}
				else{

				}
				CloseHandle(hFile);

				file_abstract_recv.filesize -= len;//更新待接收文件大小
				if (file_abstract_recv.filesize <= 0){//文件接收完整
					MessageBox(CString("write file success!"), CString("server"), MB_OK);
					//通知对方成功接收文件
					//To do？？？？？？？？？？？？？？？？
				}

			}


			break;
		case FD_CLOSE:
			tran_sock.transock = 0;
			tran_sock.current_state = 0;
			break;
		}
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void Cauthentication_clientDlg::OnBnClickedButtonUploadfile()
{
	CString filepath = "./sendfile/client.txt";//待完善，需改成灵活的文件路径名
	if (3 == current_state){//判断是否为工作状态
		UpdateData(TRUE);
		int len, retval, retval2;
		CString tempBuf;
		LONG32 fileSize;
		HANDLE hFile = CreateFile(filepath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE){
			MessageBox(CString("read file error! fail to upload!"), CString("client"), MB_OK);
			return;
		}
		fileSize = GetFileSize(hFile, NULL);

		file_abstract_send.filepath[0]='c';
		file_abstract_send.filepath[1] = 'l';
		file_abstract_send.filepath[2] = 'i';
		file_abstract_send.filepath[3] = 'e';
		file_abstract_send.filepath[4] = 'n';
		file_abstract_send.filepath[5] = 't';
		file_abstract_send.filepath[6] = '.';
		file_abstract_send.filepath[7] = 't';
		file_abstract_send.filepath[8] = 'x';
		file_abstract_send.filepath[9] = 't';
		file_abstract_send.filepath[10] = '\0';
		file_abstract_send.filesize = fileSize;
		len = 11;
		if (0 != len){
			data_to_server.Type = type_upload_DataToServer;
			retval = send(main_sock, (char*)&data_to_server, sizeof(data_to_server), 0);
			retval = send(main_sock, (char*)&file_abstract_send, sizeof(file_abstract_send), 0);
			if (retval == -1){
				retval = WSAGetLastError();
				if (retval != WSAEWOULDBLOCK){
					closesocket(main_sock);
					current_state = 0;
					main_sock = 0;
					WSACleanup();
					DisplayText = _T("");
					UpdateData(FALSE);
					MessageBox(CString("upload error! closed connection!"), CString("client"), MB_OK);
				}
			}
			else{

				MessageBox(CString("begin to upload!")+file_abstract_send.filepath, CString("client"), MB_OK);
				if (tran_sock.current_state == 0){
					retval = startTransConnect();
					if (retval == -1){
						MessageBox(CString("fail to start file transfer connection"), CString("client"), MB_OK);
						return;
					}
				}
				//文件传输连接建立成功，准备文件并发送
				DWORD dwRead;
				char *sendFileBuf = new char[fileSize];
				bool bRet = ReadFile(hFile, sendFileBuf, fileSize, &dwRead, NULL);
				if (bRet == FALSE){
					MessageBox(CString("fail to read the file: ") + file_abstract_send.filepath, CString("client"), MB_OK);
						
				}
				else if (dwRead == 0){//实际读取字节数
					MessageBox(CString("fail to read the file: ") + file_abstract_send.filepath, CString("client"), MB_OK);
				}
				else{
					//先发送文件头部信息（只需包含用户名信息）
					file_trans_head.transType = transType_upload;
					retval2 = send(tran_sock.transock, file_trans_head.clientName, sizeof(file_trans_head), 0);
					//发送文件
					retval2 = send(tran_sock.transock, sendFileBuf, dwRead, 0);
					if (retval2 == -1){
						retval2 = WSAGetLastError();
						if (retval2 != WSAEWOULDBLOCK){
							closesocket(tran_sock.transock);
							tran_sock.transock = 0;
							tran_sock.current_state = 0;
							MessageBox(CString("file transfer error!"), CString("client"), MB_OK);
						}
					}
					else{
						MessageBox(CString("file transfer success!"), CString("client"), MB_OK);
					}
				}
				CloseHandle(hFile);
				delete sendFileBuf;

			}
	
		}
		else{
			MessageBox(CString("You haven`t chosen any file to upload!"), CString("client"), MB_OK);
		}
	}
	else if (0 == current_state){
		MessageBox(CString("no connection to server!"), CString("client"), MB_OK);
	}
	else{
		MessageBox(CString("can`t upload now!"), CString("client"), MB_OK);
	}

}


void Cauthentication_clientDlg::OnBnClickedButtonDownloadfile()
{
	// TODO: Add your control notification handler code here
}

int Cauthentication_clientDlg::startTransConnect()
{
	int retval;
	sockaddr_in server_trans;
	UpdateData(TRUE);

	if (TransPort > 65535){//检查端口范围
		MessageBox(CString("transport number error!"), CString("client"), MB_OK);
		return -1;
	}

	tran_sock.transock = socket(AF_INET, SOCK_STREAM, 0);//文件传输套接字
	WSAAsyncSelect(tran_sock.transock, m_hWnd, UM_SOCK_TRANS, FD_CLOSE | FD_WRITE);

	server_trans.sin_family = AF_INET;
	RemoteIP.GetAddress(server_trans.sin_addr.S_un.S_un_b.s_b1, server_trans.sin_addr.S_un.S_un_b.s_b2, server_trans.sin_addr.S_un.S_un_b.s_b3, server_trans.sin_addr.S_un.S_un_b.s_b4);
	server_trans.sin_port = htons(TransPort);

	retval = connect(tran_sock.transock, (sockaddr*)&server_trans, sizeof(server_trans));
	if (retval == -1){
		retval = WSAGetLastError();
		if (retval != WSAEWOULDBLOCK){
			closesocket(tran_sock.transock);
			tran_sock.transock = 0;
			tran_sock.current_state = 0;
			MessageBox(CString("can`t start a connection for file transfer!"), CString("client"), MB_OK);
			return -1;
		}
		else{
			MessageBox(CString("connect success for file transfer!"), CString("client"), MB_OK);
		}
	}
	else{
		MessageBox(CString("connect success for file transfer!"), CString("client"), MB_OK);
	}
	return 0;
}