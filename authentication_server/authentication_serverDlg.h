
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
	
	struct DataFromClient
	{
		#define type_result 0
		#define type_chat_DataFromClient   1
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
		char count;
		char current_state1[maxSubSockNum];//0 等待接收连接请求 1 等待接收验证回复信息 2 工作状态
		SOCKET subSockArray[maxSubSockNum];
		CString clientName[maxSubSockNum];
		CString clientDlg[maxSubSockNum];
		struct DataToClient data_to_client[maxSubSockNum];
	};
	struct subSock sub_sock;


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
};
