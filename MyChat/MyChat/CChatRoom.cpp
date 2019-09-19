// CChatRoom.cpp: 实现文件
//

#include "stdafx.h"
#include "MyChat.h"
#include "CChatRoom.h"
#include "afxdialogex.h"
#include "MyChatDlg.h"
#include "CHomeDlg.h"
#include<vector>
// CChatRoom 对话框
//两个线程
//一个发消息给服务器
//一个接收服务器的消息
//     服务器的广播消息:欢迎来到XX聊天室，消息发送成功
//		XXX加入聊天室
#define WM_UPDATEDATA 10000 + 1
std::vector<CString> CChatRoom::m_members;
std::vector<CString> CChatRoom::m_msg;
IMPLEMENT_DYNAMIC(CChatRoom, CDialogEx)

CChatRoom::CChatRoom(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATROOM, pParent)
	, m_responce(_T(""))
	, m_chat(_T(""))
{

}

CChatRoom::~CChatRoom()
{
}

void CChatRoom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_memberList);
	DDX_Text(pDX, IDC_RESPONCE, m_responce);
	DDX_Control(pDX, IDC_LIST1, m_chatList);
	DDX_Text(pDX, IDC_EDIT_CHAT, m_chat);
}


BEGIN_MESSAGE_MAP(CChatRoom, CDialogEx)
	ON_MESSAGE(WM_UPDATEDATA, &CChatRoom::OnUpdatedata)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatRoom::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CChatRoom::OnBnClickedButtonQuit)
END_MESSAGE_MAP()



BOOL CChatRoom::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetWindowText(CHomeDlg::m_crooms->m_RoomName);

	
	m_memberList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_chatList.SetExtendedStyle(LVS_EX_FULLROWSELECT );
	m_memberList.InsertColumn(0, L"成员", 0, 100);
	

	m_chatList.InsertColumn(0, L"聊天", 0, 235);
	m_chatList.InsertColumn(1, CMyChatDlg::m_uname, LVCFMT_RIGHT, 235);
	//新建线程用来接收消息
	//CreateThread(0, 0, recvProc, (LPVOID)this, 0, 0);
	//设置定时刷新
	SetTimer(0x10002, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//自定义消息函数
afx_msg LRESULT CChatRoom::OnUpdatedata(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}


void CChatRoom::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent== 0x10002)
	{
		//如果列表控件个数和vector不同
		if (m_memberList.GetItemCount()!=m_members.size())
		{
			m_memberList.DeleteAllItems();
		
			for (auto&i:m_members)
			{
				m_memberList.InsertItem(0,i);
			}
		}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


void CChatRoom::OnClose()
{

}


void CChatRoom::OnBnClickedButtonSend()
{
	//控件->变量
	UpdateData(TRUE);
	//本客户端最后一行插入
	int n = m_chatList.GetItemCount();
	m_chatList.InsertItem(n, L"");
	m_chatList.SetItemText(n, 1, m_chat);
	//CStringA buffer = CW2A(m_chat).m_szBuffer;
	CStringA buffer = CClientSocket::CStrW2CStrA(m_chat);
	CMyChatDlg::m_clientSocket.send(sendMultiMsg, buffer,buffer.GetLength());
	m_chat = "";
	UpdateData(FALSE);
}


void CChatRoom::OnBnClickedButtonQuit()
{
	//发送离开房间消息
	//离开的房间名字
	CStringA buffer = CW2A(CHomeDlg::m_crooms->m_RoomName).m_szBuffer;
	CMyChatDlg::m_clientSocket.send(leaveRoom, buffer, buffer.GetLength());
	EndDialog(0);
	
}
