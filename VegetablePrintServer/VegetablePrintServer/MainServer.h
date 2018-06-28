#pragma once

#include "Common.h"




class MainServer
{
public:
	MainServer();

	~MainServer();

	void Initialize();

	void Send(SOCKET clientSock, string &msg);

	void SendToClientForVegeInfo(SOCKET clientSock);

	void SendToClientForBuyerInfo(SOCKET clientSock);

	void SendToClientForSearchInfo(SOCKET clientSock, int searchIndex, string searchKeyWords);

	void OnRequest(void *lpData);

private:
	
	void ErrorHandling(char *message);

	const string GetVegeInfoFromMySQL();

	const string GetBuyerInfoFromMySQL();

	const string GetSearchInfoFromMySQL(int searchIndex, string searchKeyWords);

public:
	HANDLE hComPort;
};

