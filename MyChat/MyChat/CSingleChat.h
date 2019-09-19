#pragma once


// CSingleChat 对话框

class CSingleChat : public CDialogEx
{
	DECLARE_DYNAMIC(CSingleChat)

public:
	CSingleChat(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSingleChat();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINGLECHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_CtlChat;
	CString m_strChat;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
