
// IMClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include<winsock.h>
#include<mysql.h>
#include<thread>
#include<iostream>
#include<WinInet.h>
using namespace std;


// CIMClientDlg �Ի���
class CIMClientDlg : public CDialogEx
{
// ����
public:
	CIMClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedButton();
	afx_msg void OnEnChangeInput();
	CEdit input0;
	CEdit input1;
	CStatic WEL;
	afx_msg void ON_LOGIN();
	afx_msg void ON_RETIRE();
	afx_msg void ON_REFRESH();
	CComboBox ListBox;
	afx_msg void ON_SEND();
	CEdit input2;
	CStatic CHATLOG;
	CComboBox filterBox;

	struct chatlog {
		char* sender;
		char* receiver;
		char* sentence;
		char* timestamp;
	};

	struct user {
		char* ID;
		char* address;
	};

	MYSQL mysql;
	chatlog *allLog = new chatlog[500];
	chatlog *filterLog = new chatlog[500];
	char* myID;
	user *userList = new user[50];
	SOCKET server;
	SOCKET client;
	SOCKADDR_IN addrSrv;
	SOCKADDR_IN addrSend;
	SOCKADDR_IN addrClient;
	int logNum = 1;
	int num;
	int myNum;
	WSADATA wsadata;
	MYSQL_RES *result;
	HINTERNET FTP;
	int online = 0;
	char* lastSender;
	std::thread::id temmieID;
	char mylog[5000];

	//��ȡ�������һ��IP������ʵ���������һ�£�
	char* getIP()
	{

		char host_name[255];
		if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR)
		{

			//printf("Error %d when getting local host name\n", WSAGetLastError());
			return   "1";
		}
		//printf("host name:%s\n", host_name);
		struct hostent *phe = gethostbyname(host_name);
		if (phe == 0)
		{

			//printf("Error host lookup\n");
			return   "1";

		}
		struct in_addr addr;
		for (int i = 0; phe->h_addr_list[i] != 0; ++i)
		{
			memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
			//printf("Address %d :%s\n", i, inet_ntoa(addr));
		}
		return inet_ntoa(addr);
	}
	//
	//void refreshChat() {
	//	char all[5000];
	//	for (int i = 0; i < receiveNum; i++) {
	//		if (allLog[i].receiver == myID) {
	//			sprintf(all, "%S\n%S:%S", all, allLog[i].sender, allLog[i].sentence);
	//		}
	//		if (allLog[i].sender == myID) {
	//			sprintf(all, "%S\n%S:%S", all, allLog[i].receiver, allLog[i].sentence);
	//		}
	//	}
	//	CString str(all);
	//	SetDlgItemTextW(NULL, WELC, str);
	//}

	////����һ�仰(ȥ�����߳�)
	//void thread_receieve() {
	//	SOCKET sockCon;
	//	int len = sizeof(SOCKADDR);
	//	char receiveBuf[400];
	//	memset(receiveBuf, 0, sizeof(receiveBuf));
	//	while (true) {
	//		sockCon = accept(server, (SOCKADDR*)&addrClient, &len);
	//		recv(sockCon, receiveBuf, 100, 0);
	//		allLog[receiveNum].sender = receiveBuf;
	//				allLog[receiveNum].receiver = myID;
	//				recv(sockCon, receiveBuf, 100, 0);
	//				allLog[receiveNum].sentence = receiveBuf;
	//				receiveNum++;
	//		Sleep(1000);
	//		closesocket(sockCon);
	//	}
	//}
	
	//��ʼ��winsock�⣬��ʼ��sql����
	void initial() {
		//WORD wVersionRequested;
		//WSADATA wsaData;
		//int err;
		//int len=sizeof(SOCKADDR);
		//wVersionRequested = MAKEWORD(1, 1);
		//err = WSAStartup(wVersionRequested, &wsaData);
		//if (err != 0) {
		//	return;
		//}

		//if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		//	WSACleanup();
		//	return;
		//}
		//server = socket(AF_INET, SOCK_STREAM, 0);
		//client = socket(AF_INET, SOCK_STREAM, 0);
		//memset(&addrSrv, 0, sizeof(SOCKADDR_IN));
		//addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		//addrSrv.sin_family = AF_INET;
		//addrSrv.sin_port = htons(6666);
		//bind(server, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
		//listen(server, 10);
		////accept(server, (SOCKADDR*)&addrClient, &len);
		//std::thread Temmie(thread_receieve);
		//Temmie.detach();
		mysql_library_init(0, NULL, NULL);
		mysql_init(&mysql);
		mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
		//mysql_real_connect(&mysql,"172.25.153.207", "im", "123123", "imserver", 3306, NULL, 0);
		mysql_real_connect(&mysql, "127.0.0.1", "root", "123123", "imserver", 3306, NULL, 0);
		//FTP=InternetOpen((LPCWSTR)("FTPgo"), INTERNET_OPEN_TYPE_DIRECT,)
		//InternetConnect()
		
	}

	//��½�����ص�½���
	int login(char* ID, char* password) {
		char temp[400];
		myID = ID;
		sprintf_s(temp, "select * from user where ID = '%s' and password = '%s' ", ID, password);
		mysql_query(&mysql, temp);
		result = mysql_store_result(&mysql);
		int rownumber = mysql_num_rows(result);
		if (rownumber != 1) return 1;
		if (rownumber == 1) {
			char **repeat = mysql_fetch_row(result);
			if (CString(repeat[2]) == CString("1")) return 2;
			char tmp[400];
			sprintf_s(tmp, "update user set address = '%s' , online = 1 where ID = '%s'", getIP(), ID);
			mysql_query(&mysql, tmp);
		}

		return 0;
	}

	//ע�ᣬ����ע����
	boolean reg(char* ID, char* password) {
		char temp[400];
		sprintf_s(temp, "select * from user where ID = '%s' ", ID);
		mysql_query(&mysql, temp);
		result = mysql_store_result(&mysql);
		int rownumber = mysql_num_rows(result);
		if (rownumber != 0) return false;
		if (rownumber == 0) {
			char tmp[400];
			sprintf_s(tmp, "insert into user values('%s','%s','%s',0)", ID, password, getIP());
			mysql_query(&mysql, tmp);
		}
		return true;
	}

	//ע��������ע�����
	boolean logout() {
		char tmp[400];
		sprintf_s(tmp, "update user set online = 0 where ID = '%s'", myID);
		int ret = mysql_query(&mysql, tmp);
		return ret;
	}

	//ˢ�µ�ǰ�û���
	boolean refresh() {
		mysql_query(&mysql, "select * from user where online = 1");
		result = mysql_store_result(&mysql);
		char **row;
		num = 0;
		while ((row = mysql_fetch_row(result))) {
			userList[num].ID = row[0];
			userList[num].address = row[2];
			num++;
		}
		return true;
	}

	//����һ�仰
	//boolean sendString(int usernumber, char* sentence) {
	//	addrSend.sin_addr.S_un.S_addr = inet_addr(userList[usernumber].address);
	//	addrSend.sin_family = AF_INET;
	//	addrSend.sin_port = htons(6666);
	//	connect(client, (SOCKADDR*)&addrSend, sizeof(SOCKADDR));
	//	char sendBuf[400];
	//	sprintf_s(sendBuf, myID);
	//	if(send(client, sendBuf, strlen(sendBuf) + 1, 0)==-1)return false;
	//	sprintf_s(sendBuf, sentence);
	//	send(client, sendBuf, strlen(sendBuf) + 1, 0);
	//	allLog[receiveNum].sender = myID;
	//	allLog[receiveNum].receiver = userList[usernumber].ID;
	//	allLog[receiveNum].sentence = sentence;
	//	receiveNum++;
	//	return true;
	//}

	//����һ���ļ�
	boolean sendFile() {
		return true;
	}
	//�µ�ˢ�������߳�
	int refreshLog() {
		char tmp[100];
		allLog = new chatlog[500];
		memset(tmp, 0, 100);
		sprintf_s(tmp, "select * from chatlog");
		if (mysql_query(&mysql, tmp))return 0;
		result = mysql_store_result(&mysql);
		logNum = mysql_num_rows(result) + 1;
		sprintf_s(tmp, "select * from chatlog where sender = '%s'  or receiver = '%s'", myID, myID);
		if (mysql_query(&mysql, tmp))return 0;
		result = mysql_store_result(&mysql);
		char **row;
		myNum = 0;
		while (row = mysql_fetch_row(result)) {
			allLog[myNum].sender = row[0];
			allLog[myNum].receiver = row[1];
			allLog[myNum].sentence = row[2];
			allLog[myNum].timestamp = row[3];
			if (string(row[0]) != string(myID) || string(row[1]) == string(row[0]))
				lastSender = row[0];
			myNum++;
		}
		return 1;
	}
	//�µķ�����Ϣ����
	int sendMessage(int num, char* sentence) {
		refreshLog();
		char tmp[100];
		memset(tmp, 0, 100);
		sprintf_s(tmp, "insert into chatlog values('%s','%s','%s',%d)", myID, userList[num].ID, sentence, logNum);
		mysql_query(&mysql, tmp);
		allLog[logNum].sender = myID;
		allLog[logNum].receiver = userList[num].ID;
		allLog[logNum].sentence = sentence;
		allLog[logNum].timestamp = (char*)logNum;
		return 1;
	}

	//�µĹ��˷���
	int refreshFliter(int num) {
		char* other = userList[num].ID;
		int k = 0;
		memset(mylog, 0, sizeof(mylog));
		for (int i = 0; i < myNum; i++) {
			if ((string(allLog[i].sender) == string(other) && string(allLog[i].receiver) == string(myID)) ||
				(string(allLog[i].sender) == string(myID) && string(allLog[i].receiver) == string(other))) {
				if(k==0)
					sprintf_s(mylog,"%s->%s:%s",allLog[i].sender, allLog[i].receiver, allLog[i].sentence);
				if(k>0)
					sprintf_s(mylog,"%s\n%s->%s:%s", mylog, allLog[i].sender, allLog[i].receiver, allLog[i].sentence);
				k++;
			}
		}
		CString Asgore(mylog);
		SetDlgItemTextW(CHAT,Asgore);
		return k;
	}
	int refreshFliter(char* other) {
		int k = 0;
		memset(mylog, 0, sizeof(mylog));
		for (int i = 0; i < myNum; i++) {
			if ((string(allLog[i].sender) == string(other) && string(allLog[i].receiver) == string(myID)) ||
				(string(allLog[i].sender) == string(myID) && string(allLog[i].receiver) == string(other))) {
				if (k == 0)
					sprintf_s(mylog, "%s->%s:%s", allLog[i].sender, allLog[i].receiver, allLog[i].sentence);
				if (k>0)
					sprintf_s(mylog, "%s\n%s->%s:%s", mylog, allLog[i].sender, allLog[i].receiver, allLog[i].sentence);
				k++;
			}
		}
		CString Asgore(mylog);
		SetDlgItemTextW(CHAT, Asgore);
		return k;
	}
	//�������Ϣ���̣߳�ÿ��������һ�Σ���WELC�ؼ��������Ϣ
	static void* kep(char* myID, MYSQL mysql, MYSQL_RES *result, int logNum, CWnd *c) {
		int m = logNum,n=0;
		char tmp[100];
		int LOVE;
		while (1) {
			memset(tmp, 0, 100);
			sprintf_s(tmp, "select * from chatlog where sender = '%s'  or receiver = '%s'", myID, myID);
			if (mysql_query(&mysql, tmp)) break;
			result = mysql_store_result(&mysql);
			char **row;
			int newNum = 0;
			int newMessageNum = 0;
			char* sender;
			while (row = mysql_fetch_row(result)) {
				if (newNum >= m) {
					m++;
					if (string(row[1]) == string(myID)) {
						sender = row[0];
						newMessageNum++;
					}
				}
				newNum++;
			}
			if (newMessageNum > 0) {
				sprintf_s(tmp,"*���յ�%d������Ϣ\n*������%s��������\n*�����㲻�뿴��",newMessageNum,sender);
				LOVE = 0;
				CString str(tmp);
				c->SetDlgItemTextW(WELC, str);
			}
			else {
				LOVE++;
				switch (LOVE)
				{
				case 1:sprintf_s(tmp, "*��������������û������Ϣ��\n*�����ٵȵȣ�");
					break;
				case 2:sprintf_s(tmp, "*ʮ��û������Ϣ����\n*��������������һ�������");
					break;
				case 3:sprintf_s(tmp, "*��ôʮ����û���˻�Ӧ������\n*�����һ��ʼ���û���ڴ�����Ӧ");
					break;
				case 4:sprintf_s(tmp, "*��ʮ��\n*���");
					break;
				case 5:sprintf_s(tmp, "*������ʮ����û�˸��㷢��Ϣ\n*�����ļһ�\n*��������Ժ��Լ����죬���£�");
					break;

				default:sprintf_s(tmp, "*�ٻ�ƣ�˵ʵ�ڵ�\n*��������Ŀ��Ժ��Լ�����\n*ȥ���԰�");
					break;
				}

				CString str(tmp);
				c->SetDlgItemTextW(WELC, str);
			}
			Sleep(5000);
			n++;
		}
		return 0;
	}
	//����һ���ļ�

	////������Ϣ
	//int filter(char* ID) {
	//	int Num = 0;
	//	for (int i = 0; i < receiveNum; i++) {
	//		if (allLog[i].receiver == ID || allLog[i].sender == ID) {
	//			filterLog[Num] = allLog[i];
	//			Num++;
	//		}
	//	}
	//	return Num;
	//}




	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void ToLastestMessage();
	CEdit CHA;
};
