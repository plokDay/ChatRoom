
// MyChatDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MyChat.h"
#include "MyChatDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include"CChatRoom.h"
#include"CClientSocket.h"
#include"CHomeDlg.h"




CClientSocket CMyChatDlg::m_clientSocket;
CString CMyChatDlg::m_uname= _T("");
// CMyChatDlg 对话框



CMyChatDlg::CMyChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, m_passwd(_T(""))
	, m_responce(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UNAME, m_uname);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_passwd);
	DDX_Text(pDX, IDC_STATIC_NOTE, m_responce);
}

BEGIN_MESSAGE_MAP(CMyChatDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_REGI, &CMyChatDlg::OnBnClickedButtonRegi)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CMyChatDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// CMyChatDlg 消息处理程序

BOOL CMyChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(L"Chat Room");
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_responce = m_clientSocket.m_responce;
	m_passwd = L"123456";
	UpdateData(FALSE);
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		//背景图片
// 		CRect rc;
// 		GetWindowRect(&rc);
// 		CImage mImage;
// 		int n = mImage.Load(_T("..\\MyChat\\res\\bg_log.jpg"));
// 		if (n == S_OK)
// 		{
// 			//这里让图片绘制区域和窗口大小一致
// 			mImage.Draw(GetDC()->GetSafeHdc(), CRect(0, 0, rc.Width(), rc.Height()));
// 		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//注册
void CMyChatDlg::OnBnClickedButtonRegi()
{
	//控件->CString
	UpdateData(TRUE);
	//拼接，宽->多
	//buffer.Format("%s\n%s", CW2A(m_uname).m_szBuffer, CW2A(m_passwd).m_szBuffer);
	CStringA buffer = CClientSocket::CStrW2CStrA(m_uname) + "\t" + CClientSocket::CStrW2CStrA(m_passwd);
	//发送
	m_clientSocket.send(registe, buffer, buffer.GetLength());
	//接收
	m_clientSocket.recv();
	m_responce = m_clientSocket.m_responce;
	UpdateData(FALSE);
}

//登录
void CMyChatDlg::OnBnClickedButtonLogin()
{
	//控件->CString
	UpdateData(TRUE);

	//拼接，宽->多
	//CStringA buffer;
	//buffer.Format("%s\n%s", CW2A(m_uname).m_szBuffer, CW2A(m_passwd).m_szBuffer);
	
	CStringA buffer = CClientSocket::CStrW2CStrA(m_uname)+"\t"+ CClientSocket::CStrW2CStrA(m_passwd);
	//发送
	m_clientSocket.send(login, buffer, buffer.GetLength());
	//接收
	m_clientSocket.recv();
	m_responce = m_clientSocket.m_responce;
	UpdateData(FALSE);
	
	if (m_responce == L"登陆成功")
	{
		ShowWindow(SW_HIDE);
		CHomeDlg homeDlg;
		homeDlg.DoModal();
		OnClose();
		exit(0);
	}
	
}

