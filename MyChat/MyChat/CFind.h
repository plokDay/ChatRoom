#pragma once


// CFind 对话框

class CFind : public CDialogEx
{
	DECLARE_DYNAMIC(CFind)

public:
	CFind(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFind();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_findName;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CListCtrl m_findRes;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	static DWORD CALLBACK RecvProc(LPVOID p);
};
