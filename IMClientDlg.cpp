
// IMClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IMClient.h"
#include "IMClientDlg.h"
#include "afxdialogex.h"
#include<winsock.h>
#include<mysql.h>
#include<thread>
#include<iostream>

using namespace std;
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libmysql.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIMClientDlg 对话框



CIMClientDlg::CIMClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIMClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, USER_INPUT, input0);
	DDX_Control(pDX, PW_INPUT, input1);
	DDX_Control(pDX, IDC_COMBO1, ListBox);
	DDX_Control(pDX, IDC_EDIT3, input2);
	DDX_Control(pDX, CHAT, CHATLOG);
	DDX_Control(pDX, IDC_COMBO2, filterBox);
}

BEGIN_MESSAGE_MAP(CIMClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(REG_BUTTON, &CIMClientDlg::OnBnClickedButton)
	ON_BN_CLICKED(LOGIN_BUTTON, &CIMClientDlg::ON_LOGIN)
	ON_BN_CLICKED(LOGOUT_BUTTON, &CIMClientDlg::ON_RETIRE)
	ON_BN_CLICKED(IDC_BUTTON1, &CIMClientDlg::ON_REFRESH)
	ON_BN_CLICKED(IDC_BUTTON7, &CIMClientDlg::ON_SEND)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CIMClientDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON2, &CIMClientDlg::ToLastestMessage)
END_MESSAGE_MAP()


// CIMClientDlg 消息处理程序

BOOL CIMClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIMClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIMClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		initial();
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
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIMClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIMClientDlg::OnBnClickedButton()
{
	CString str;
	char *ID=new char[40],*pw=new char[40];
	char welcome[400];
	int ret;
	if (online == 1) {
		sprintf_s(welcome, "*你试图在已经登陆的时候注册账号\n  但是什么都没有发生");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
		return;
	}
	input1.GetWindowTextW(str);
	sprintf(pw,"%S", str);//用这个转换的时候必须用大写S
	input0.GetWindowTextW(str);
	sprintf(ID, "%S", str);
	if ( reg(ID, pw)) {
		sprintf_s(welcome, "*那么你就算是注册成功了\n*虽然有点麻烦\n*然后你还需要点一下登陆？");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
	}
	else {
		sprintf_s(welcome, "*这是一个别人的名字呢\n*或许你应该想一个自己的名字\n*快想！");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
	}
}


void CIMClientDlg::ON_LOGIN()
{

	CString str;
	char *ID = new char[40], *pw = new char[40];
	char welcome[400];
	int ret;
	if (online == 1) {
		sprintf_s(welcome, "*如果你已经登陆过了\n*那么你为什么还要登陆呢\n*真是麻烦");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
		return;
	}
	input1.GetWindowTextW(str);
	sprintf(pw, "%S", str);//用这个转换的时候必须用大写S
	input0.GetWindowTextW(str);
	sprintf(ID, "%S", str);
	if ((ret = login(ID, pw)) == 0) {;
		myID = ID;
		sprintf_s(welcome, "*你登录了\n*你登录成功了\n*你的名字是%s?",myID);
		CString str2(welcome);
		online = 1;
		SetDlgItemText(WELC, str2);
		sprintf_s(welcome, "当前用户:%s", myID);
		CString str3(welcome);
		SetWindowTextW(str3);
		Sleep(5000);
		refresh();
		refreshLog();
		ListBox.ResetContent();
		filterBox.ResetContent();
		for (int i = 0; i < num; i++) {
			CString  str(userList[i].ID);
			ListBox.InsertString(i, str);
			filterBox.InsertString(i, str);
		}
		CWnd* a = this;
		std::thread temmie(kep, std::ref(myID), std::ref(mysql), std::ref(result), std::ref(myNum), std::ref(a));
		temmieID = temmie.get_id();
		temmie.detach();
	}
	else {
		sprintf_s(welcome, "*你输入了一组账号密码\n*你试图登陆但是失败了\n*你难道连你的名字都记不住吗");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
	}
}


void CIMClientDlg::ON_RETIRE()
{
	if (online != 1) {
		char welcome[400];
		sprintf_s(welcome, "*你甚至还没有登陆\n*你就这么想RESET吗？\n*即使是现在这个世界");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
		return;
	}
	logout();
	char welcome[400];
	ListBox.ResetContent();
	filterBox.ResetContent();
	myID = "";
	online = 0;
	sprintf_s(welcome, "*你真的要注销吗？\n*看来你已经注销了\n*那好吧，回见");
	CString str2(welcome);
	sprintf_s(welcome, "IMClient");
	CString str3(welcome);
	SetDlgItemText(WELC, str2);
	SetWindowTextW(str3);
}


void CIMClientDlg::ON_REFRESH()
{
	if (online != 1) {
		char welcome[400];
		sprintf_s(welcome, "*虽然我没有提示过\n*但这个按钮不登录是不能点的\n*或许我曾经还认为你有点常识");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
		return;
	}
	refresh();
	refreshLog();
	ListBox.ResetContent();
	filterBox.ResetContent();
	for (int i = 0; i < num; i++) {
		CString  str(userList[i].ID);
		ListBox.InsertString(i, str);
		filterBox.InsertString(i, str);
	}
}


void CIMClientDlg::ON_SEND()
{
	if (online != 1){
		char welcome[400];
		sprintf_s(welcome, "*如果你不登录的话 \n*那么你是谁呢\n*回见");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
		return;
	}
	int num = ListBox.GetCurSel();
	CString str;
	char sentence[400];
	input2.GetWindowTextW(str);
	sprintf(sentence, "%S", str);
	if (sendMessage(num, sentence)) {
		sprintf(sentence, "*看起来这个句子发出去了 \n*我顺便帮你切换了窗口\n*不过右边这个框好丑啊");
		CString str2(sentence);
		SetDlgItemText(WELC, str2);
	}
	refreshLog();
	refreshFliter(num);
}


void CIMClientDlg::OnCbnSelchangeCombo2()
{
	refreshLog();
	refreshFliter(filterBox.GetCurSel());
}


void CIMClientDlg::ToLastestMessage()
{
	if (online != 1) {
		char welcome[400];
		sprintf_s(welcome, "*如果你不登录的话\n*你想要查看给谁的信息呢\n*找到你的名字再来见我");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
		return;
	}
	refreshLog();
	refreshFliter(lastSender);
}
