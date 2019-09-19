#pragma once

#include <vector>
// CChatRoom 对话框

class CChatRoom : public CDialogEx
{
	DECLARE_DYNAMIC(CChatRoom)

public:
	CChatRoom(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChatRoom();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATROOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
 	static std::vector<CString> m_members;
	static std::vector<CString> m_msg;

	CListCtrl m_memberList;
	CString m_responce;
protected:
	afx_msg LRESULT OnUpdatedata(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	CListCtrl m_chatList;
	CString m_chat;
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonQuit();
};
