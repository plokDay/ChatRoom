// CSingleChat.cpp: 实现文件
//

#include "stdafx.h"
#include "MyChat.h"
#include "CSingleChat.h"
#include "afxdialogex.h"
#include "MyChatDlg.h"
#include "CHomeDlg.h"
// CSingleChat 对话框

IMPLEMENT_DYNAMIC(CSingleChat, CDialogEx)

CSingleChat::CSingleChat(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SINGLECHAT, pParent)
	, m_strChat(_T(""))
{

}

CSingleChat::~CSingleChat()
{
}

void CSingleChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CtlChat);
	DDX_Text(pDX, IDC_EDIT1, m_strChat);
}


BEGIN_MESSAGE_MAP(CSingleChat, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSingleChat::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSingleChat::OnBnClickedButton2)
END_MESSAGE_MAP()


// CSingleChat 消息处理程序


BOOL CSingleChat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(L" To " + CHomeDlg::m_cfriend->m_strSendTo);
	m_CtlChat.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_CtlChat.InsertColumn(0, CHomeDlg::m_cfriend->m_strSendTo,0,200);
	m_CtlChat.InsertColumn(1, CMyChatDlg::m_uname, 0, 200);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//发送消息
void CSingleChat::OnBnClickedButton1()
{
	//控件->变量
	UpdateData(TRUE);
	//本客户端最后一行插入
	int n = m_CtlChat.GetItemCount();
	m_CtlChat.InsertItem(n, L"");
	m_CtlChat.SetItemText(n, 1, m_strChat);
	
	//发送私聊消息   des\n content
 	
	CStringA buffer = CClientSocket::CStrW2CStrA(CHomeDlg::m_cfriend->m_strSendTo) +
		"\t" + CClientSocket::CStrW2CStrA(m_strChat);
 	//buffer.Format("%s\n%s", CW2A(CHomeDlg::m_cfriend->m_strSendTo).m_szBuffer, CW2A(m_strChat).m_szBuffer);
 	CMyChatDlg::m_clientSocket.send(sendSingle, buffer, buffer.GetLength());
	//发送保存聊天记录的消息
	//chatFrom\n chatTo\n chatContent
	buffer = CClientSocket::CStrW2CStrA(CMyChatDlg::m_uname) +
		"\t" + CClientSocket::CStrW2CStrA(CHomeDlg::m_cfriend->m_strSendTo)+"\t"+ CClientSocket::CStrW2CStrA(m_strChat);
	//buffer.Format("%s\n%s\n%s", CW2A(CMyChatDlg::m_uname).m_szBuffer, CW2A(CHomeDlg::m_cfriend->m_strSendTo).m_szBuffer, CW2A(m_strChat).m_szBuffer);
	CMyChatDlg::m_clientSocket.send(saveChat, buffer, buffer.GetLength());

	//编辑框置空
	m_strChat = "";
	UpdateData(FALSE);
}

//聊天记录非模态
void CSingleChat::OnBnClickedButton2()
{
	CHomeDlg::m_record->Create(IDD_RECORD,this);
	CHomeDlg::m_record->ShowWindow(SW_SHOW);
}
