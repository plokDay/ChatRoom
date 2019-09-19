
// MyChatDlg.h: 头文件
//
#include"CClientSocket.h"
#pragma once


// CMyChatDlg 对话框
class CMyChatDlg : public CDialogEx
{
// 构造
public:
	CMyChatDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	static CString m_uname;
	CString m_passwd;
	CString m_responce;
	static CClientSocket m_clientSocket;
public:
	afx_msg void OnBnClickedButtonRegi();
	afx_msg void OnBnClickedButtonLogin();
	
	
};
