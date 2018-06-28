#ifndef COMMON_H
#define COMMON_H

using namespace std;
#include <winsock2.h> 
#include <iostream>


#define _CRT_SECURE_NO_WARNINGS  

#include <cstdio>  
#include <cstdlib>  
#include <process.h>  

#include <windows.h> 
#include <string>
#include <memory.h>
#include "MessageDefine.h"
#include "Utility.h"


#pragma comment(lib,"ws2_32.lib") //加载ws2_32.dll  

#pragma warning(disable : 4996)  


#define BUF_SIZE 65535  

// typedef struct    // socket info  
// {
// 	SOCKET hClntSock;
// 	SOCKADDR_IN clntAdr;
// } PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

enum IO_OPERATION{IO_READ, IO_WRITE};

typedef struct    // buffer info  
{
	OVERLAPPED		overlapped;
	WSABUF			wsaBuf;
	IO_OPERATION	rwMode;    // READ or WRITE 读写模式  
	int				nBytes;
	SOCKET			sock;
} IO_DATA, *LP_IO_DATA;


// 数据库连接的一些配置
const string MYSQLDBUSER		= "root";
const string MYSQLDBPASSWORD	= "123456"; // it must be    changed
const string MYSQLDBIP			= "localhost";
const string MYSQLDBNAME		= "vegetables";

const int VEGETABLESTABLE_COLUMN_COUNT		= 2;
const int BUYERTABLE_COLUMN_COUNT			= 5;
const int SEARCH_THROUGH_NAME_COLUMN_COUNT	= 3;

#endif