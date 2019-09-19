// CHomeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MyChat.h"
#include "CHomeDlg.h"
#include "afxdialogex.h"
#include"MyChatDlg.h"
#include "CSingleChat.h"
#include "CRooms.h"
#include "CRecord.h"

// CHomeDlg 对话框
#define WM_UPDATEDATA 10000 + 1
CChatRoom* CHomeDlg::m_room=new CChatRoom;
CSingleChat* CHomeDlg::m_single=new CSingleChat;
CFriend* CHomeDlg::m_cfriend = new CFriend;
CRooms* CHomeDlg::m_crooms = new CRooms;
CRecord* CHomeDlg::m_record = new CRecord;
IMPLEMENT_DYNAMIC(CHomeDlg, CDialogEx)

CHomeDlg::CHomeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOME, pParent)
	, m_userName(_T(""))
	, m_userSex(_T(""))
	, m_userDate(_T(""))
{

}

CHomeDlg::~CHomeDlg()
{
}

void CHomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Text(pDX, IDC_EDIT1, m_userName);
	DDX_Text(pDX, IDC_EDIT2, m_userSex);
	DDX_Text(pDX, IDC_EDIT3, m_userDate);
}


BEGIN_MESSAGE_MAP(CHomeDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CHomeDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CHomeDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_MESSAGE(WM_UPDATEDATA, &CHomeDlg::OnUpdatedata)
END_MESSAGE_MAP()


// CHomeDlg 消息处理程序

DWORD CALLBACK CHomeDlg::RecvProc(LPVOID p)
{
	CHomeDlg* dlg = (CHomeDlg*)p;
	CFind* findDlg = dlg->m_cfind;
	CMsg* msgDlg = dlg->m_cmsg;
	CFriend* friendDlg = dlg->m_cfriend;
	CChatRoom* roomDlg = dlg->m_room;
	CSingleChat* singleDlg = dlg->m_single;
	CRooms* roomsDlg = dlg->m_crooms;
	CRecord* recordDlg = dlg->m_record;
	//接收消息
	while (true)
	{
		CMyChatDlg::m_clientSocket.recv();
		DataPackResult* pBuff = CMyChatDlg::m_clientSocket.m_respPack;

		if (pBuff != nullptr)
		{
			//getUserInfo
			if (pBuff->type == 19 && pBuff->status == -1)
			{
				CString buffer;
				//按\t切割
				AfxExtractSubString(buffer, (CString)pBuff->data, 0, '\t');
				dlg->m_userName = buffer;
				AfxExtractSubString(buffer, (CString)pBuff->data, 1, '\t');
				dlg->m_userSex = buffer;
				AfxExtractSubString(buffer, (CString)pBuff->data, 2, '\t');
				dlg->m_userDate = buffer;
				//不能用
				dlg->SendMessage(WM_UPDATEDATA, FALSE);
			}
			//FindAll
			if (pBuff->type == 7 && pBuff->status == -1)
			{

				if ((CString)pBuff->data == L"没找到")
				{
					findDlg->m_findRes.InsertItem(0, L"Null");
					findDlg->m_findRes.SetItemText(0, 1, L"Null");
					continue;
				}
				CString buffer;
				//按空格切割
				AfxExtractSubString(buffer, (CString)pBuff->data, 0, ' ');
				findDlg->m_findRes.InsertItem(0, buffer);
				AfxExtractSubString(buffer, (CString)pBuff->data, 1, ' ');
				findDlg->m_findRes.SetItemText(0, 1, buffer);

			}
			//msgList
			if ((pBuff->status == -1) && (pBuff->type == 9))
			{
				std::vector<CString>().swap(msgDlg->m_msgVect);
				CString buffer; int i = 0;
				//按空格切割
				while (buffer != "" || i <= 1)
				{
					AfxExtractSubString(buffer, (CString)pBuff->data, i, ',');
					++i;
					if (buffer != "")
					{
						msgDlg->m_msgVect.push_back(buffer);
					}
				}
			
			}
			//getFriendList
			if ((pBuff->status == -1) && (pBuff->type == 11))
			{
				std::vector<CString>().swap(friendDlg->m_friendVect);
				CString buffer; int i = 0;
				//按空格切割
				while (buffer != "" || i <= 1)
				{
					AfxExtractSubString(buffer, (CString)pBuff->data, i, ',');
					++i;
					if (buffer != "")
					{
						friendDlg->m_friendVect.push_back(buffer);
					}
				}
			}
			if ((pBuff->status == -1) && (pBuff->type == 4))//notify,XXX加入了聊天室
			{
				roomDlg->m_responce = CMyChatDlg::m_clientSocket.m_responce;
				//dlg->UpdateData(FALSE);
				roomDlg->SendMessage(WM_UPDATEDATA, FALSE);
			}
			//room_members
			if (pBuff->status == 4 && pBuff->type == 4)
			{
				std::vector<CString>().swap(roomDlg->m_members);
				CString buffer; int i = 0;
				//按空格切割
				while (buffer != "" || i <= 1)
				{
					AfxExtractSubString(buffer, (CString)pBuff->data, i, ' ');
					++i;
					if (buffer != "")
					{
						roomDlg->m_members.push_back(buffer);
					}
				}

			}
			if (pBuff->status == -1 && pBuff->type == 3)//MultiMsg聊天室消息
			{
				//dlg->m_msg.push_back((CString)pBuff->data);
				roomDlg->m_chatList.InsertItem(roomDlg->m_chatList.GetItemCount(), (CString)pBuff->data);
			}
			//私聊消息
			if ((pBuff->status == -1) && (pBuff->type == 13))
			{
				singleDlg->m_CtlChat.InsertItem(singleDlg->m_CtlChat.GetItemCount(), (CString)pBuff->data);
			}
			//getRoomList
			if ((pBuff->status == -1) && (pBuff->type == 14))
			{
				CString buffer; int i = 0;
				//按空格切割
				while (buffer != "" || i <= 1)
				{
					AfxExtractSubString(buffer, (CString)pBuff->data, i, ' ');
					++i;
					if (buffer != "")
					{
						roomsDlg->m_roomsVect.push_back(buffer);
					}
				}
			}
			//checkRecord
			if (pBuff->status == -1 && pBuff->type == 18)
			{
				CString buffer; int i = 0;
				//按空格切割
				while (buffer != "" || i <= 1)
				{
					AfxExtractSubString(buffer, (CString)pBuff->data, i);//默认是\n
					++i;
					if (buffer != "")
					{
						int nIdx = recordDlg->m_listRecord.GetItemCount();
						CString buff;
						AfxExtractSubString(buff, buffer, 0, '\t');
						recordDlg->m_listRecord.InsertItem(nIdx, buff+L":");
						AfxExtractSubString(buff, buffer, 1, '\t');
						recordDlg->m_listRecord.SetItemText(nIdx, 1, buff);
						AfxExtractSubString(buff, buffer, 2, '\t');
						recordDlg->m_listRecord.SetItemText(nIdx, 2, buff);
					}
				}
			}
			//isFriend
			if (pBuff->type == 22 && pBuff->status == -1)
			{
				if ((CString)pBuff->data == L"No")
				{
					friendDlg->MessageBox(L"你们不是好友", L"", MB_OK);
					friendDlg->m_isFriend = 0;
				}
			}
		}
		
	}
}
BOOL CHomeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(CMyChatDlg::m_uname+L"的主页");
	
	addTab(L"好友", m_cfriend, IDD_FRIEND);
	addTab(L"聊天室", m_crooms, IDD_ROOMS);
	addTab(L"查找", m_cfind, IDD_FIND);
	addTab(L"验证消息", m_cmsg, IDD_MSG);
	m_pTabSubWnd[0]->ShowWindow(SW_SHOW);
	//发送拉取用户信息的消息
	CStringA buffer = CW2A(CMyChatDlg::m_uname).m_szBuffer;
	CMyChatDlg::m_clientSocket.send(getUserInfo, buffer, buffer.GetLength());

	SetTimer(0, 5000, 0);
	//创建线程接收消息
	CreateThread(0, 0, RecvProc, (LPVOID)this, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CHomeDlg::OnClose()
{
	CStringA buffer = CW2A(CMyChatDlg::m_uname).m_szBuffer;
	//发送下线消息
	CMyChatDlg::m_clientSocket.send(logout, buffer, buffer.GetLength());

	CDialogEx::OnClose();
}


void CHomeDlg::addTab(const CString& title,CDialogEx* pSubWnd,UINT uId)
{
	m_tab.InsertItem(m_tab.GetItemCount(), title);
	pSubWnd->Create(uId, &m_tab);
	CRect rect;
	m_tab.GetClientRect(rect);
	rect.DeflateRect(1, 23, 1, 1);
	pSubWnd->MoveWindow(rect);
	m_pTabSubWnd.push_back(pSubWnd);
}


void CHomeDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
 	for (auto&i:m_pTabSubWnd)
 	{
 		i->ShowWindow(SW_HIDE);
 	}
	int n = m_tab.GetCurSel();
	m_pTabSubWnd[n]->ShowWindow(SW_SHOW);
}

//更改用户信息
void CHomeDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if (m_userSex!=L"男"&& m_userSex != L"保密"&& m_userSex != L"女")
	{
		MessageBox(L"请输入男/女/保密", 0, MB_YESNO);
		return;
	}
	//发送更改信息消息
	CStringA buffer =CClientSocket::CStrW2CStrA(m_userSex);
	CMyChatDlg::m_clientSocket.send(updateUserInfo, buffer, buffer.GetLength());

}


void CHomeDlg::OnTimer(UINT_PTR nIDEvent)
{
	//UpdateData(FALSE);

	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CHomeDlg::OnUpdatedata(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}
