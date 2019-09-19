// CRooms.cpp: 实现文件
//

#include "stdafx.h"
#include "MyChat.h"
#include "CRooms.h"
#include "afxdialogex.h"
#include "MyChatDlg.h"
#include "CChatRoom.h"
#include "CHomeDlg.h"
// CRooms 对话框

IMPLEMENT_DYNAMIC(CRooms, CDialogEx)

CRooms::CRooms(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROOMS, pParent)
	, m_RoomName(_T(""))
{

}

CRooms::~CRooms()
{
}

void CRooms::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_strRoomName);
	DDX_Text(pDX, IDC_EDIT1, m_RoomName);
	DDX_Control(pDX, IDC_LIST1, m_roomList);
}


BEGIN_MESSAGE_MAP(CRooms, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRooms::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRooms::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CRooms::OnNMDblclkList1)
END_MESSAGE_MAP()


// CRooms 消息处理程序

//创建聊天室
void CRooms::OnBnClickedButton1()
{
	//发送创建聊天室的消息
	//roomName
	UpdateData(TRUE);
	if (m_RoomName == "") return;
	CStringA buffer = CW2A(m_RoomName).m_szBuffer;

	CMyChatDlg::m_clientSocket.send(createRoom, buffer, buffer.GetLength());

	//编辑框置空
	//m_RoomName = "";
	//UpdateData(FALSE);
	CEdit*  pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);//获取相应的编辑框ID
	pEdit->SetWindowText(_T("")); //设置默认显示的内容 
	CHomeDlg::m_room->DoModal();
}

//刷新房间消息
void CRooms::OnBnClickedButton2()
{
	//发送更新列表的消息
	CMyChatDlg::m_clientSocket.send(updateRooms, "dd", 0);
	// 发送getRoomList消息
	m_roomList.DeleteAllItems();
	std::vector<CString>().swap(m_roomsVect);
	CStringA buffer = CClientSocket::CStrW2CStrA(CMyChatDlg::m_uname);
	//CStringA buffer = CW2A(CMyChatDlg::m_uname).m_szBuffer;
	CMyChatDlg::m_clientSocket.send(getRoomList, buffer, buffer.GetLength());
}


void CRooms::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==0x1010)
	{
		//如果列表控件个数和vector不同
		if (m_roomList.GetItemCount() != m_roomsVect.size())
		{
			m_roomList.DeleteAllItems();

			for (auto&i : m_roomsVect)
			{
				
				CString buffer;
				AfxExtractSubString(buffer, i, 0, ',');
				
				m_roomList.InsertItem(0, buffer);
				AfxExtractSubString(buffer, i,1, ',');
				m_roomList.SetItemText(0, 1, buffer);

			}
		}
	}
// 	if (nIDEvent == 0x1011)
// 	{
// 		//发送更新列表的消息
// 		CMyChatDlg::m_clientSocket.send(updateRooms, "dd", 0);
// 		// 发送getRoomList消息
// 		m_roomList.DeleteAllItems();
// 		std::vector<CString>().swap(m_roomsVect);
// 		CStringA buffer = CClientSocket::CStrW2CStrA(CMyChatDlg::m_uname);
// 		//CStringA buffer = CW2A(CMyChatDlg::m_uname).m_szBuffer;
// 		CMyChatDlg::m_clientSocket.send(getRoomList, buffer, buffer.GetLength());
// 	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CRooms::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_roomList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_roomList.InsertColumn(0, L"名称", 0, 80);
	m_roomList.InsertColumn(1, L"人数", 0, 80);

	SetTimer(0x1010, 3500, 0);
	SetTimer(0x1011, 3000, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRooms::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nIdx = (int)m_roomList.GetFirstSelectedItemPosition();
	if (nIdx == 0) {
		return;
	}
	nIdx -= 1;
	m_RoomName = m_roomList.GetItemText(nIdx, 0);
	if (m_RoomName == "Null") return;
	//发送加入房间的消息
	//member\n addRoomName
	//buffer.Format("%s\n%s", CW2A(CMyChatDlg::m_uname).m_szBuffer, CW2A(m_RoomName).m_szBuffer);
	CStringA buffer = CClientSocket::CStrW2CStrA(CMyChatDlg::m_uname)+"\t"+ CClientSocket::CStrW2CStrA(m_RoomName);
	CMyChatDlg::m_clientSocket.send(addRoom, buffer, buffer.GetLength());
	
	CHomeDlg::m_room->DoModal();
	*pResult = 0;
}
