
// authentication_serverDlg.h : header file
//

#pragma once
#include "afxwin.h"


// Cauthentication_serverDlg dialog
class Cauthentication_serverDlg : public CDialogEx
{
// Construction
public:
	Cauthentication_serverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AUTHENTICATION_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	SHORT LocalPort;
	UINT password;
	struct mainSock
	{
		SOCKET mainsock;
		bool current_state0;//0 无服务 1 有服务
	};
	struct mainSock main_sock;

	struct tranSock
	{
		SOCKET transock;
		bool current_state;//0 无服务 1 有服务
	};
	struct tranSock tran_sock;

	struct DataFromClient
	{
		#define type_result 0
		#define type_chat_DataFromClient   1
		#define type_upload_DataFromClient 2
		char Type;
		UINT queryResult;
		CString content;
	};
	struct DataFromClient data_from_client;

	struct DataToClient
	{
		#define type_query 0
		#define type_reply 1
		#define type_chat_DataToClient  2
		#define valid CString("valid")
		#define invalid CString("invalid")
		char Type;
		USHORT count_2byte;
		USHORT count_4byte;
		USHORT * data_2byte;
		UINT * data_4byte;
		CString content;
	};

	struct subSock
	{
		#define maxSubSockNum 64
		UINT count;
		char current_state1[maxSubSockNum];//0 等待接收连接请求 1 等待接收验证回复信息 2 工作状态
		SOCKET subSockArray[maxSubSockNum];
		CString clientName[maxSubSockNum];
		CString clientDlg[maxSubSockNum];
		struct DataToClient data_to_client[maxSubSockNum];
	};
	struct subSock sub_sock;

	/*file abstract*/
	struct FileAbstract
	{
		char filepath[100];
		LONG32 filesize;
	};
	struct FileAbstract file_abstract;

	/*temporary transfer record Buffer for every client*/
	struct TTRB
	{
		#define maxNumOfTTRB 64
		UINT	count;//有效记录数
		CString clientName[maxNumOfTTRB];//用户名
		SOCKET transock[maxNumOfTTRB];//传输套接字
		CString filepath[maxNumOfTTRB];//接收存储路径文件名
		LONG32 filesize[maxNumOfTTRB];//待接收文件大小
	};
	struct TTRB ttrb;
	CString tempFilePath = "./recvfile/";

	afx_msg void OnBnClickedButtonStartserver();
	afx_msg void OnBnClickedButtonStopserver();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButtonSendmsg();
	afx_msg void OnCbnDblclkCurrentClient();
	afx_msg LRESULT UpdateCurrentList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT UpdateCurrentClient(WPARAM wParam, LPARAM lParam);
	CButton	server_start;
	CString SendMsgText;
	CString DisplayText;
	CString CurrentList;
	CComboBox CurrentClient;
	char CurrentClientMsgFlag;//0:add 1:delete 2:flush
	afx_msg void OnCbnEditchangeCurrentClient();
	afx_msg void OnCbnDropdownCurrentClient();
	afx_msg void OnBnClickedButtonUploadfile();
	afx_msg void OnBnClickedButtonDownloadfile();
	int Cauthentication_serverDlg::startTranServer();
	int Cauthentication_serverDlg::initTTRB();
	int Cauthentication_serverDlg::deleteTTRB(int index);
	int Cauthentication_serverDlg::searchTTRB(SOCKET);
	int Cauthentication_serverDlg::searchTTRB(CString);
	int Cauthentication_serverDlg::findFreeTTRB();
};
