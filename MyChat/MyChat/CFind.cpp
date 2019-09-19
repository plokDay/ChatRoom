// CFind.cpp: 实现文件
//

#include "stdafx.h"
#include "MyChat.h"
#include "CFind.h"
#include "afxdialogex.h"
#include "MyChatDlg.h"

// CFind 对话框

IMPLEMENT_DYNAMIC(CFind, CDialogEx)

CFind::CFind(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FIND, pParent)
	, m_findName(_T(""))
{

}

CFind::~CFind()
{
}

void CFind::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_findName);
	DDX_Control(pDX, IDC_LIST1, m_findRes);
}


BEGIN_MESSAGE_MAP(CFind, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CFind::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFind::OnBnClickedButton2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CFind::OnNMDblclkList1)
END_MESSAGE_MAP()


// CFind 消息处理程序


void CFind::OnBnClickedButton1()
{
	//将控件的值更新到变量
	UpdateData(TRUE);
	CStringA buffer = CW2A(m_findName).m_szBuffer;
	//发送消息
	CMyChatDlg::m_clientSocket.send(findAll, buffer, buffer.GetLength());
	
	
}


BOOL CFind::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_findRes.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_findRes.InsertColumn(0, L"用户名", 0, 80);
	m_findRes.InsertColumn(1, L"是否在线", 0, 80);

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CFind::OnBnClickedButton2()
{
	m_findRes.DeleteAllItems();
}


void CFind::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nIdx = (int)m_findRes.GetFirstSelectedItemPosition();
	if (nIdx == 0) {
		return;
	}
	nIdx -= 1;
	CString strName = m_findRes.GetItemText(nIdx, 0);
	if (strName == "Null") return;
	if (MessageBox(L"是否添加" + strName + "为好友", L"添加好友", MB_YESNO)==IDYES)
	{
		CStringA buffer;
		buffer.Format("%s\t%s", CW2A(CMyChatDlg::m_uname).m_szBuffer, CW2A(strName).m_szBuffer);
		
		//发送加好友消息
		CMyChatDlg::m_clientSocket.send(addFriend, buffer, buffer.GetLength());
	}
	*pResult = 0;
}
