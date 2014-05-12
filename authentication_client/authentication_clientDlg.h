
// authentication_clientDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "string"

// Cauthentication_clientDlg dialog
class Cauthentication_clientDlg : public CDialogEx
{
// Construction
public:
	Cauthentication_clientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AUTHENTICATION_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartconnect();
	afx_msg void OnBnClickedButtonStopconnect();
	afx_msg void OnBnClickedButtonSendmsg();
	int Cauthentication_clientDlg::startTransConnect();
	UINT password;
	CIPAddressCtrl RemoteIP;
	SHORT RemotePort;
	SHORT TransPort;
	CString SendMsgText;
	CString DisplayText;
	SOCKET main_sock;

	struct tranSock
	{
		SOCKET transock;
		bool current_state;//0 �޷��� 1 �з���
	};
	struct tranSock tran_sock;

	struct DataFromServer
	{
		#define type_query 0
		#define type_reply 1
		#define type_chat_DataFromServer  2
		#define valid CString("valid")
		#define invalid CString("invalid")
		char Type;
		USHORT count_2byte;
		USHORT count_4byte;
		USHORT * data_2byte;
		UINT * data_4byte;
		CString content;
	};
	struct DataFromServer data_from_server;

	struct DataToServer
	{
		#define type_result 0
		#define type_chat_DataToServer 1
		#define type_upload_DataToServer 2
		char Type;
		UINT queryResult;
		CString content;
	};
	struct DataToServer data_to_server;
	/*file abstract*/
	struct FileAbstract
	{
		char filepath[100];
		LONG32 filesize;
	};
	struct FileAbstract file_abstract;

/*	typedef enum{
		STATE0 = 0,//��ʼ״̬
		STATE1,//�ȴ����ӽ��״̬
		STATE2,//�ȴ���ѯ����״̬
		STATE3,//�ȴ���֤���״̬
		STATE4,//����״̬
	}STATE;
	STATE current_state;
	*/
	char current_state;//0 ��ʼ״̬ 1 �ȴ���ѯ����״̬ 2 �ȴ���֤���״̬ 3 ����״̬
	afx_msg void OnBnClickedButtonUploadfile();
	afx_msg void OnBnClickedButtonDownloadfile();
};
