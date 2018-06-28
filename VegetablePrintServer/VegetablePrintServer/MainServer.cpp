#include "MainServer.h"

#include "SQLClient.h"

unsigned int  WINAPI EchoThreadMain(LPVOID CompletionPortIO);


char buffer[BUF_SIZE];


MainServer::MainServer()
{
}


MainServer::~MainServer()
{
}


void MainServer::Initialize()
{
	WSADATA wsaData;
	SYSTEM_INFO sysInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int  i;
	DWORD recvBytes = 0, flags = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	// 连接数据库
	SQLClient::getInstance()->ConnectMySQL();

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);//创建CP对象  
	GetSystemInfo(&sysInfo);//获取当前系统的信息  

	for (i = 0; i < sysInfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, EchoThreadMain, this, 0, NULL);//创建=CPU个数的线程数  

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);//不是非阻塞套接字，但是重叠IO套接字。  
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(12345);

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, SOMAXCONN);

	while (1)
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);
		// 1. 阻塞等待客户端的连接
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);

		cout << "a new client connect!" << endl;
		// 2. 将新连接上的客户端sock与完成端口进行绑定
		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, 0, 0);//连接套接字和CP对象  

		SendToClientForVegeInfo(hClntSock);

// 		IO_DATA *ioInfo = new IO_DATA;
// 		memset(buffer, 0, BUF_SIZE);
// 		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
// 		ioInfo->wsaBuf.len = sizeof(buffer);
// 		ioInfo->wsaBuf.buf = buffer;//和重叠IO一样  
// 		ioInfo->rwMode = IO_READ;//读写模式  
// 		ioInfo->sock = hClntSock;
// 
// 		SendToClientForVegeInfo(hClntSock);
// 
// 		int nRet = WSARecv(hClntSock, &(ioInfo->wsaBuf), 1, 
// 			&recvBytes, &flags, &(ioInfo->overlapped), NULL);
// 
// 		if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()))
// 		{
// 			cout << "WSARecv Failed::Reason Code:: " << WSAGetLastError() << endl;
// 			closesocket(hClntSock);
// 			delete ioInfo;
// 		}
// 		else
// 		{
// 			cout << "the recv info: " << ioInfo->wsaBuf.buf << endl;
// 		}
	}
}

void MainServer::Send(SOCKET clientSock, string &msg)
{
	IO_DATA *ioInfo = new IO_DATA;
	memset(&ioInfo->overlapped, 0, sizeof(ioInfo->overlapped));
	ioInfo->wsaBuf.buf = const_cast<char *>(msg.c_str());
	ioInfo->wsaBuf.len = msg.size() + 1;
	ioInfo->rwMode = IO_WRITE;
	ioInfo->nBytes = msg.size() + 1;
	ioInfo->sock = clientSock;
	DWORD nBytes = msg.size() + 1;
	int nRet = WSASend(clientSock, &ioInfo->wsaBuf, 1, &nBytes, 0, &(ioInfo->overlapped), NULL);
	if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()))
	{
		cout << "WASSend Failed::Reason Code::" << WSAGetLastError() << endl;
		closesocket(clientSock);
		delete ioInfo;
		return;
	}
	memset(buffer, NULL, sizeof(buffer));
}

void MainServer::SendToClientForVegeInfo(SOCKET clientSock)
{
	// 1. 从数据库中获取菜品信息
	string vegeInfo = GetVegeInfoFromMySQL();
	if (vegeInfo == "")
	{
		cout << "select vege table error" << endl;
		return;
	}
	cout << "select vege info success!" << endl;

	// 2. 将数据发送给客户端
	Send(clientSock, vegeInfo);
}

void MainServer::SendToClientForBuyerInfo(SOCKET clientSock)
{
	// 1. 从数据库中获取买家信息
	string buyerInfo = GetBuyerInfoFromMySQL();
	if (buyerInfo == "")
	{
		cout << "select buyer table error" << endl;
		return;
	}
	cout << "select buyer info success!" << endl;

	// 2. 将数据发送给客户端
	Send(clientSock, buyerInfo);
}

void MainServer::SendToClientForSearchInfo(SOCKET clientSock, int searchIndex, string searchKeyWords)
{
	// 1. 从数据库中获取买家信息
	string searchInfo = GetSearchInfoFromMySQL(searchIndex, searchKeyWords);
	if (searchInfo == "")
	{
		cout << "select search table error" << endl;
		return;
	}
	cout << "select search info success!" << endl;
	cout << searchInfo << endl;
	// 2. 将数据发送给客户端
	Send(clientSock, searchInfo);
}

unsigned int WINAPI EchoThreadMain(LPVOID lpParameter)//线程的执行  
{
	MainServer *lpServer = (MainServer *)lpParameter;

	HANDLE hComPort = lpServer->hComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LP_IO_DATA ioInfo;
	DWORD nBytes;
	DWORD dwFlags;
	void *lpCompletionKey = NULL;
	int nRet = 0;

	while (1)//大循环  
	{
		// LPDWORD
		GetQueuedCompletionStatus(hComPort, &bytesTrans,//确认“已完成”的I/O！！  
			(PULONG_PTR)&lpCompletionKey, (LPOVERLAPPED*)&ioInfo, INFINITE);//INFINITE使用时，程序将阻塞，直到已完成的I/O信息写入CP对象  

		if (bytesTrans == 0)
		{
			cout << "client disconnect " << endl;
			closesocket(ioInfo->sock);
			delete ioInfo;
			continue;
		}
		cout << "ioInfo->rwMode: " << ioInfo->rwMode << endl;
		
		if (ioInfo->rwMode == IO_READ)//读写模式（此时缓冲区有数据）  
		{
			puts("message received!");

			lpServer->OnRequest(ioInfo);			

// 			cout << "ioInfo->buf: " << ioInfo->wsaBuf.buf << endl;
// 			memset(&ioInfo->overlapped, 0, sizeof(ioInfo->overlapped));
// 			ioInfo->wsaBuf.buf = buffer;
// 			ioInfo->wsaBuf.len = strlen(buffer) + 1;
// 			ioInfo->rwMode = IO_WRITE;
// 			ioInfo->nBytes = strlen(buffer) + 1;
// 			dwFlags = 0;
// 			nBytes = strlen(buffer) + 1;
// 			nRet = WSASend(
// 				ioInfo->sock,
// 				&ioInfo->wsaBuf, 1, &nBytes,
// 				dwFlags,
// 				&(ioInfo->overlapped), NULL);
// 			if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) {
// 				cout << "WASSend Failed::Reason Code::" << WSAGetLastError() << endl;
// 				closesocket(ioInfo->sock);
// 				delete ioInfo;
// 				continue;
// 			}
// 			memset(buffer, NULL, sizeof(buffer));
		}
		else if (ioInfo->rwMode == IO_WRITE)
		{
			puts("message send!");
			ioInfo->rwMode = IO_READ;
			nBytes = BUF_SIZE;
			dwFlags = 0;
			ioInfo->wsaBuf.buf = buffer;
			ioInfo->wsaBuf.len = nBytes;
			ioInfo->nBytes = nBytes;
			memset(&ioInfo->overlapped, 0, sizeof(ioInfo->overlapped));

			nRet = WSARecv(ioInfo->sock, &ioInfo->wsaBuf, 1, &nBytes, &dwFlags, &ioInfo->overlapped, NULL);
			if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) {
				cout << "WASRecv Failed::Reason Code1::" << WSAGetLastError() << endl;
				closesocket(ioInfo->sock);
				delete ioInfo;
				continue;
			}
		}
		
	}
	return 0;
}

void MainServer::ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

const string MainServer::GetVegeInfoFromMySQL()
{
	string vegeInfo = SQLClient::getInstance()->GetVegeInfo();
	if (vegeInfo == "")
	{
		const char* str = "select * from tblvegetableprice";
		vegeInfo = SQLClient::getInstance()->SelectDataFromMySQL(str, VEGETABLESTABLE_COLUMN_COUNT);
		SQLClient::getInstance()->SetVegeInfo(vegeInfo);
	}
	return vegeInfo;
}

const string MainServer::GetBuyerInfoFromMySQL()
{
	string buyerInfo = SQLClient::getInstance()->GetBuyerInfo();
	if (buyerInfo == "")
	{
		const char* str = "select * from tblbuyerinfo";
		buyerInfo = SQLClient::getInstance()->SelectDataFromMySQL(str, BUYERTABLE_COLUMN_COUNT);
		SQLClient::getInstance()->SetBuyerInfo(buyerInfo);
	}
	return buyerInfo;
}

const std::string MainServer::GetSearchInfoFromMySQL(int searchIndex, string searchKeyWords)
{
	string searchInfo;
	if (searchIndex == 0)
	{// 按照姓名查找，简单
		char str[60] = "";
		sprintf(str, "select * from %s", searchKeyWords.c_str());

		searchInfo = SQLClient::getInstance()->SelectDataFromMySQL(str, SEARCH_THROUGH_NAME_COLUMN_COUNT);
		SQLClient::getInstance()->SetBuyerInfo(searchInfo);
	}
	else
	{
		
	}
	

	return searchInfo;
}

void MainServer::OnRequest(void *lpData)
{
	LP_IO_DATA ioInfo = (LP_IO_DATA)lpData;
	vector<string> curMsg = split(ioInfo->wsaBuf.buf);

	if (curMsg.size() != 0)
	{
		int msgID = atoi(curMsg[0].c_str());

		switch (msgID)
		{
		case REQUEST_BUYERINFO:
			SendToClientForBuyerInfo(ioInfo->sock);
			break;
		case REQUEST_SEARCH:
			SendToClientForSearchInfo(ioInfo->sock, atoi(curMsg[1].c_str()), curMsg[2]);
			break;
		default:
			//memset(&ioInfo->overlapped, 0, sizeof(ioInfo->overlapped));
			
			break;
		}

		memset(buffer, NULL, sizeof(buffer));
	}
}