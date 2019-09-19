#pragma once
#include<vector>
#include"CFriend.h"
#include"CRooms.h"
#include"CFind.h"
#include"CMsg.h"
#include "CChatRoom.h"
#include "CSingleChat.h"
#include "CRecord.h"
// CHomeDlg 对话框

class CHomeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHomeDlg)

public:
	CHomeDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHomeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CTabCtrl m_tab;
	std::vector<CDialogEx*> m_pTabSubWnd;
	CFind* m_cfind = new CFind;
	static CFriend* m_cfriend;
	static CRooms* m_crooms;
	CMsg* m_cmsg = new CMsg;
	static CChatRoom* m_room;
	static CSingleChat* m_single;
	static CRecord* m_record;
private:
	void addTab(const CString& title, CDialogEx* pSubWnd, UINT uId);

public:
	static DWORD CALLBACK RecvProc(LPVOID p);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_userName;
	CString m_userSex;
	CString m_userDate;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnUpdatedata(WPARAM wParam, LPARAM lParam);
};
