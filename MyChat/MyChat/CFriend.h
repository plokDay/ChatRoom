#pragma once
#include <vector>

// CFriend 对话框

class CFriend : public CDialogEx
{
	DECLARE_DYNAMIC(CFriend)

public:
	CFriend(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFriend();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FRIEND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMenu m_menu;
	bool m_isFriend = 1;
	std::vector<CString> m_friendVect;
	CString m_strSendTo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	CListCtrl m_friList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeleteFriend();
};
