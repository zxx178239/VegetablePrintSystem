#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <windows.h>
#include <process.h>
#include <queue>
#include "common.h"

#define NAME_SIZE 20

using namespace std;

//#pragma comment(lib, "ws2_32.lib")

class SocketClient
{
public:
    SocketClient();


    ~SocketClient();

    void Initialize();


    void PushToSendQueue(char *msg);

    char *RecvFromServer();

    void CloseSock();

    void OnRequest(void *lpData);

    void Lock();

    void UnLock();

public:

    SOCKET hSock;

    std::queue<char *> sendQueue;

    std::queue<char *> recvQueue;

    HANDLE hSndThread;
    HANDLE hRcvThread;

    CRITICAL_SECTION locker;
};

#endif // SOCKETCLIENT_H
