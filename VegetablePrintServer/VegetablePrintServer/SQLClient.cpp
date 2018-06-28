#include "SQLClient.h"



SQLClient::~SQLClient()
{
	CloseMySQL();
}

int SQLClient::ConnectMySQL()
{
	vegeInfos = "";
	buyerInfos = "";

	mysqlConn = mysql_init(NULL);
	if (mysqlConn == NULL)
	{
		MessageBoxA(NULL, "mysql_init error, please check the mysql!", "", NULL);
		return -1;
	}

	mysqlConn = mysql_real_connect(mysqlConn, MYSQLDBIP.c_str(), MYSQLDBUSER.c_str(), MYSQLDBPASSWORD.c_str(), 
									MYSQLDBNAME.c_str(), 3306, NULL, 0);

	if (mysqlConn)
	{
		cout << "mysql connect success!" << endl;
	}
	else
	{
		cout << "mysql connect failed!" << endl;
	}

	if (mysql_set_character_set(mysqlConn, "GBK") != 0)
	{
		return -1;
	}
	return 0;
}


void SQLClient::CloseMySQL()
{
	mysql_close(mysqlConn);
}

string SQLClient::SelectDataFromMySQL(const char *selectSentence, int columnCount)
{
	MYSQL_ROW m_row;
	MYSQL_RES *m_res;

	char rg = ',';//行隔开
	char cg = '\n';//字段隔开

	string str;

	if (mysql_query(mysqlConn, selectSentence) != 0)
	{
		MessageBoxA(NULL, "select ps_info Error!", "", NULL);
		return "";
	}
	m_res = mysql_store_result(mysqlConn);

	if (m_res == NULL)
	{
		MessageBoxA(NULL, "select username Error!", "", NULL);
		return "";
	}

	while (m_row = mysql_fetch_row(m_res))
	{
		for (int i = 0; i < columnCount; i++)
		{
			// 主要考虑到可能有些字段为空
			if (m_row[i] == NULL)
				str += "";
			else
				str += m_row[i];
			if (i + 1 != columnCount)
			{
				str += rg;
			}
		}
		str += cg;
	}

	mysql_free_result(m_res);

	return str;
}

string SQLClient::GetVegeInfo()
{
	return vegeInfos;
}

string SQLClient::GetBuyerInfo()
{
	return buyerInfos;
}

void SQLClient::SetVegeInfo(string selectInfo)
{
	vegeInfos = selectInfo;
}

void SQLClient::SetBuyerInfo(string selectInfo)
{
	buyerInfos = selectInfo;
}
