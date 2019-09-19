// CFriend.cpp: 实现文件
//

#include "stdafx.h"
#include "MyChat.h"
#include "CFriend.h"
#include "afxdialogex.h"
#include "MyChatDlg.h"
#include "CHomeDlg.h"
#include "resource.h"
// CFriend 对话框

IMPLEMENT_DYNAMIC(CFriend, CDialogEx)

CFriend::CFriend(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FRIEND, pParent)
{

}

CFriend::~CFriend()
{
}

void CFriend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_friList);
}


BEGIN_MESSAGE_MAP(CFriend, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CFriend::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CFriend::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CFriend::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CFriend::OnDeleteFriend)
END_MESSAGE_MAP()


// CFriend 消息处理程序


void CFriend::OnTimer(UINT_PTR nIDEvent)
{
	
 	if (nIDEvent==0x1005)
 	{
 		//发送消息，获取好友
 		CStringA buffer = CW2A(CMyChatDlg::m_uname).m_szBuffer;
 		CMyChatDlg::m_clientSocket.send(getFriendList, buffer, buffer.GetLength());
 	}
 	//刷新
 	if (nIDEvent == 0x1006)
 	{
 		//如果列表控件个数和vector不同
 		//if (m_friList.GetItemCount() != m_friendVect.size())
 		{
 			m_friList.DeleteAllItems();
 
 			for (auto&i : m_friendVect)
 			{
				CString buffer;
				AfxExtractSubString(buffer, i, 0, ' ');
				m_friList.InsertItem(0, buffer);
				AfxExtractSubString(buffer, i, 1, ' ');
				m_friList.SetItemText(0, 1, buffer);
 				//m_friList.InsertItem(0, i);
 			}
 		}
 	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CFriend::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_menu.LoadMenu(IDR_DELETEF);
	m_friList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_friList.InsertColumn(0, L"好友", 0, 100);
	m_friList.InsertColumn(1, L"是否在线", 0, 100);
	SetTimer(0x1005, 2000, NULL);
	SetTimer(0x1006, 2500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CFriend::OnBnClickedButton1()
{
	//发送消息，获取好友
	CStringA buffer = CW2A(CMyChatDlg::m_uname).m_szBuffer;
	CMyChatDlg::m_clientSocket.send(getFriendList, buffer, buffer.GetLength());
}


void CFriend::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nIdx = (int)m_friList.GetFirstSelectedItemPosition();
	if (nIdx == 0) {
		return;
	}
	nIdx -= 1;
	m_strSendTo = m_friList.GetItemText(nIdx, 0);
	//检测是不是好友
	CStringA buffer = CW2A(m_strSendTo).m_szBuffer;
	CMyChatDlg::m_clientSocket.send(isFriend, buffer, buffer.GetLength());
	
	if (m_isFriend==0)
	{
		return;
	}
	CString m_isOnline= m_friList.GetItemText(nIdx, 1);
	if (m_isOnline==L"0")
	{
		//MessageBox(L"不在线", 0, MB_OK);
		return;
	}
	
	if (m_strSendTo == "Null") return;
	
	//打开私聊窗口
	CHomeDlg::m_single->DoModal();
	
	*pResult = 0;
}


void CFriend::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu* subMenu = m_menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	subMenu->TrackPopupMenu(0, pos.x, pos.y, this);
	*pResult = 0;
}


void CFriend::OnDeleteFriend()
{
	int index = (int)m_friList.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;


	CString friName = m_friList.GetItemText(index, 0);// 第1列是
	
	//发送删除好友消息
	CStringA buffer = CW2A(friName).m_szBuffer;
	CMyChatDlg::m_clientSocket.send(deleteFriend, buffer, buffer.GetLength());

	
}
