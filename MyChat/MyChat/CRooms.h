#pragma once
#include<vector>

// CRooms 对话框

class CRooms : public CDialogEx
{
	DECLARE_DYNAMIC(CRooms)

public:
	CRooms(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRooms();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROOMS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	std::vector<CString> m_roomsVect;
	afx_msg void OnBnClickedButton1();
	CEdit m_strRoomName;
	CString m_RoomName;//要创建的房间的名字
	CListCtrl m_roomList;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
