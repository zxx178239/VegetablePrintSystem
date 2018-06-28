#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <windows.h>
#include <process.h>
#include "common.h"

#define NAME_SIZE 20

//#pragma comment(lib, "ws2_32.lib")

class SocketClient
{
public:
    SocketClient();


    ~SocketClient();

    void Initialize();


    void SendToServer(void *msg);

    char *RecvFromServer();

    void CloseSock();

private:

    SOCKET hSock;

    char buffer[BUF_SIZE];
};

#endif // SOCKETCLIENT_H
