// CMsg.cpp: 实现文件
//

#include "stdafx.h"
#include "MyChat.h"
#include "CMsg.h"
#include "afxdialogex.h"
#include "MyChatDlg.h"


// CMsg 对话框

IMPLEMENT_DYNAMIC(CMsg, CDialogEx)

CMsg::CMsg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MSG, pParent)
{

}

CMsg::~CMsg()
{
}

void CMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_msgList);
}


BEGIN_MESSAGE_MAP(CMsg, CDialogEx)
	ON_WM_TIMER()
	
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CMsg::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMsg::OnBnClickedButton1)
END_MESSAGE_MAP()



// CMsg 消息处理程序

BOOL CMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_msgList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_msgList.InsertColumn(0, L"待处理",0,200);
	SetTimer(0x1003, 5500, NULL);
	SetTimer(0x1004, 4000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMsg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==0x1003)
	{
		//发送获取消息列表
 		//CStringA buffer = CW2A(CMyChatDlg::m_uname).m_szBuffer;
  		//CMyChatDlg::m_clientSocket.send(getFriendMsg, buffer, buffer.GetLength());

	}

	if (nIDEvent == 0x1004)
	{
		//如果列表控件个数和vector不同
		if (m_msgList.GetItemCount() != m_msgVect.size())
		{
			m_msgList.DeleteAllItems();

			for (auto&i : m_msgVect)
			{
				m_msgList.InsertItem(0, i);
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}




void CMsg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nIdx = (int)m_msgList.GetFirstSelectedItemPosition();
	if (nIdx == 0) {
		return;
	}
	nIdx -= 1;
	CString strName = m_msgList.GetItemText(nIdx, 0);
	
	if (strName == "Null") return;
	if (MessageBox(L"是否同意", 0, MB_YESNO) == IDYES)
	{
		CString buff;
		//空格分割得到发出申请的人
		AfxExtractSubString(buff, strName, 0, ' ');
		//    
		CStringA buffer;
		buffer.Format("%s\t%s", CW2A(buff).m_szBuffer, CW2A(CMyChatDlg::m_uname).m_szBuffer);

		//发送加好友消息   主动方/n被动方
		CMyChatDlg::m_clientSocket.send(agreeFriend, buffer, buffer.GetLength());

		//发送消息，销毁这条记录
	}
	//处理完之后发送消息，删除该验证信息
	CStringA buf = CW2A(strName).m_szBuffer;
	CMyChatDlg::m_clientSocket.send(deleteMsg, buf, buf.GetLength());
	std::vector<CString>::iterator iter = std::find(m_msgVect.begin(), m_msgVect.end(), strName);
	m_msgVect.erase(iter);
	*pResult = 0;
}


void CMsg::OnBnClickedButton1()
{
	
	//发送获取消息列表
	CStringA buffer = CW2A(CMyChatDlg::m_uname).m_szBuffer;
	CMyChatDlg::m_clientSocket.send(getFriendMsg, buffer, buffer.GetLength());
}
