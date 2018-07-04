#include "socketclient.h"


unsigned WINAPI SendMsg(void * arg);//发送信息函数
unsigned WINAPI RecvMsg(void * arg);//接受信息函数
void ErrorHandling(char * msg);//错误返回函数

int haveread = 0;

SocketClient::SocketClient()
{

}

SocketClient::~SocketClient()
{
    WaitForSingleObject(hSndThread, INFINITE);//等待线程结束
    WaitForSingleObject(hRcvThread, INFINITE);

    DeleteCriticalSection(&locker);
}

void SocketClient::Initialize()
{
    WSADATA wsaData;

    SOCKADDR_IN servAdr;
    //HANDLE hSndThread, hRcvThread;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    InitializeCriticalSection(&locker);

    hSock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAdr.sin_port = htons(12345);

    if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("connect() error");

    cout << "connect success!" << endl;


    hSndThread =
        (HANDLE)_beginthreadex(NULL, 0, SendMsg, this, 0, NULL);//写线程
    hRcvThread =
        (HANDLE)_beginthreadex(NULL, 0, RecvMsg, this, 0, NULL);//读线程

}


unsigned WINAPI SendMsg(void * arg)   // send thread main
{
    SocketClient *lpClient = (SocketClient *)arg;

    SOCKET sock = lpClient->hSock;

    while (1)
    {
        lpClient->Lock();

        if(lpClient->sendQueue.empty())
        {
            lpClient->UnLock();
            continue;
        }
        else
        {
            char *sendMsg = lpClient->sendQueue.front();
            lpClient->sendQueue.pop();
            lpClient->UnLock();
            send(sock, sendMsg, strlen(sendMsg), 0);
            cout << "send msg" << endl;
        }
    }
    return NULL;
}

unsigned WINAPI RecvMsg(void * arg)  // read thread main
{
    SocketClient *lpClient = (SocketClient *)arg;

    SOCKET sock = lpClient->hSock;
    char buffer[BUF_SIZE] = "";

    while (1)
    {
        memset(buffer, 0, BUF_SIZE);
        recv(sock, buffer, BUF_SIZE, 0);
        lpClient->OnRequest(buffer);
    }
    return NULL;
}

void SocketClient::OnRequest(void *lpData)
{
    vector<string> curMsg = split((char *)lpData, ' ');

    if(curMsg.size() != 0)
    {
        int msgID = atoi(curMsg[0].c_str());
        char *msg = substr((char *)lpData);
        switch(msgID)
        {
            case NOTIFY_VEGETABLEINFO:
                cout << "NOTIFY_VEGETABLEINFO" << endl;
                DataManager::getInstance()->saveVegeInfos(msg);
                break;
            case  NOTIFY_BUYER_INFO:
                cout << "NOTIFY_BUYER_INFO" << endl;
                DataManager::getInstance()->saveBuyerInfos(msg);
                break;
            case NOTIFY_SEARCH_INFO:
                cout << "NOTIFY_SEARCH_INFO" << endl;
                break;
        }
        free(msg);
    }

}


void SocketClient::PushToSendQueue(char *msg)
{
    Lock();
    sendQueue.push(msg);
    UnLock();
}


void SocketClient::CloseSock()
{
    closesocket(hSock);
    WSACleanup();
}

void ErrorHandling(char * msg)
{

    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}


void SocketClient::Lock()
{
    EnterCriticalSection(&locker);
}

void SocketClient::UnLock()
{
    LeaveCriticalSection(&locker);
}
