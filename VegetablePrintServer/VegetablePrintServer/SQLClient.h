#pragma once

#include "Common.h"
#include "mysql.h"


class SQLClient
{
public:
	static SQLClient *getInstance()
	{
		static SQLClient mySQLClient;
		return &mySQLClient;
	}

	~SQLClient();


	int ConnectMySQL();

	void CloseMySQL();

	string SelectDataFromMySQL(const char *selectSentence, const int columnCount);

	string GetVegeInfo();

	string GetBuyerInfo();

	void SetVegeInfo(string selectInfo);

	void SetBuyerInfo(string selectInfo);

private:
	SQLClient(){};
	
private:
	MYSQL *mysqlConn;

	string vegeInfos;

	string buyerInfos;
};

