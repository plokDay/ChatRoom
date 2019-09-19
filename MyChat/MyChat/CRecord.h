#pragma once


// CRecord 对话框

class CRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CRecord)

public:
	CRecord(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRecord();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listRecord;
	virtual BOOL OnInitDialog();
};
