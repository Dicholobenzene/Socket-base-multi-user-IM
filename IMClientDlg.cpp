
// IMClientDlg.cpp : ʵ���ļ�
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





// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CIMClientDlg �Ի���



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


// CIMClientDlg ��Ϣ�������

BOOL CIMClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIMClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		initial();
		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		sprintf_s(welcome, "*����ͼ���Ѿ���½��ʱ��ע���˺�\n  ����ʲô��û�з���");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
		return;
	}
	input1.GetWindowTextW(str);
	sprintf(pw,"%S", str);//�����ת����ʱ������ô�дS
	input0.GetWindowTextW(str);
	sprintf(ID, "%S", str);
	if ( reg(ID, pw)) {
		sprintf_s(welcome, "*��ô�������ע��ɹ���\n*��Ȼ�е��鷳\n*Ȼ���㻹��Ҫ��һ�µ�½��");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
	}
	else {
		sprintf_s(welcome, "*����һ�����˵�������\n*������Ӧ����һ���Լ�������\n*���룡");
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
		sprintf_s(welcome, "*������Ѿ���½����\n*��ô��Ϊʲô��Ҫ��½��\n*�����鷳");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
		return;
	}
	input1.GetWindowTextW(str);
	sprintf(pw, "%S", str);//�����ת����ʱ������ô�дS
	input0.GetWindowTextW(str);
	sprintf(ID, "%S", str);
	if ((ret = login(ID, pw)) == 0) {;
		myID = ID;
		sprintf_s(welcome, "*���¼��\n*���¼�ɹ���\n*���������%s?",myID);
		CString str2(welcome);
		online = 1;
		SetDlgItemText(WELC, str2);
		sprintf_s(welcome, "��ǰ�û�:%s", myID);
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
		sprintf_s(welcome, "*��������һ���˺�����\n*����ͼ��½����ʧ����\n*���ѵ���������ֶ��ǲ�ס��");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
	}
}


void CIMClientDlg::ON_RETIRE()
{
	if (online != 1) {
		char welcome[400];
		sprintf_s(welcome, "*��������û�е�½\n*�����ô��RESET��\n*��ʹ�������������");
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
	sprintf_s(welcome, "*�����Ҫע����\n*�������Ѿ�ע����\n*�Ǻðɣ��ؼ�");
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
		sprintf_s(welcome, "*��Ȼ��û����ʾ��\n*�������ť����¼�ǲ��ܵ��\n*��������������Ϊ���е㳣ʶ");
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
		sprintf_s(welcome, "*����㲻��¼�Ļ� \n*��ô����˭��\n*�ؼ�");
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
		sprintf(sentence, "*������������ӷ���ȥ�� \n*��˳������л��˴���\n*�����ұ������ó�");
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
		sprintf_s(welcome, "*����㲻��¼�Ļ�\n*����Ҫ�鿴��˭����Ϣ��\n*�ҵ����������������");
		CString str2(welcome);
		SetDlgItemText(WELC, str2);
		return;
	}
	refreshLog();
	refreshFliter(lastSender);
}
