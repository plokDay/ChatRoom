// CRecord.cpp: 实现文件
//

#include "stdafx.h"
#include "MyChat.h"
#include "CRecord.h"
#include "afxdialogex.h"
#include "MyChatDlg.h"
#include "CHomeDlg.h"


// CRecord 对话框

IMPLEMENT_DYNAMIC(CRecord, CDialogEx)

CRecord::CRecord(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RECORD, pParent)
{

}

CRecord::~CRecord()
{
}

void CRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listRecord);
}


BEGIN_MESSAGE_MAP(CRecord, CDialogEx)
END_MESSAGE_MAP()


// CRecord 消息处理程序


BOOL CRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(L"聊天记录");
	m_listRecord.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listRecord.InsertColumn(0, L"人物", 0, 50);
	m_listRecord.InsertColumn(1, L"内容", 0, 250);
	m_listRecord.InsertColumn(2, L"时间", 0, 250);


	//发送查看聊天记录的消息
	////chatFrom\n chatTo
	CStringA buffer;
	buffer.Format("%s\t%s", CW2A(CMyChatDlg::m_uname).m_szBuffer, CW2A(CHomeDlg::m_cfriend->m_strSendTo).m_szBuffer);
	CMyChatDlg::m_clientSocket.send(checkRecord, buffer, buffer.GetLength());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
