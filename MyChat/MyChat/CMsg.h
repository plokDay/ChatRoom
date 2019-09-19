#pragma once
#include <vector>

// CMsg 对话框

class CMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsg)

public:
	CMsg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMsg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	std::vector<CString> m_msgVect;
	virtual BOOL OnInitDialog();
	CListCtrl m_msgList;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
};
