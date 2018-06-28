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

}

void SocketClient::Initialize()
{
    WSADATA wsaData;

    SOCKADDR_IN servAdr;
    //HANDLE hSndThread, hRcvThread;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    hSock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAdr.sin_port = htons(12345);

    if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("connect() error");

    cout << "connect success!" << endl;

    /*
    hSndThread =
        (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);//写线程
    hRcvThread =
        (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);//读线程

    WaitForSingleObject(hSndThread, INFINITE);//等待线程结束
    WaitForSingleObject(hRcvThread, INFINITE);
    */
}

/*
unsigned WINAPI SendMsg(void * arg)   // send thread main
{
    SOCKET sock = *((SOCKET*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    char padd[2];
    fgets(padd, 2, stdin);//多余的'\n'
    printf("\n send message:");
    while (1)
    {
        {
            fgets(msg, BUF_SIZE, stdin);
            if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
            {
                closesocket(sock);
                exit(0);
            }
            sprintf(name_msg, "[%s] %s", NAME, msg);
            char numofmsg = strlen(name_msg) + '0';
            char newmsg[100]; newmsg[0] = numofmsg; newmsg[1] = 0;//第一个字符表示消息的长度
            strcat(newmsg, name_msg);
            int result = send(sock, newmsg, strlen(newmsg), 0);
            if (result == -1)return -1;//发送错误
        }
    }
    return NULL;
}

unsigned WINAPI RecvMsg(void * arg)  // read thread main
{
    SOCKET sock = *((SOCKET*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    int str_len = 0;
    while (1)
    {
        {
            char lyfstr[1000] = { 0 };
            int totalnum = 0;
            str_len = recv(sock, name_msg, 1, 0);//读取第一个字符！获取消息的长度
            if (str_len == -1)//读取错误
            {
                printf("return -1\n");
                return -1;
            }
            if (str_len == 0)//读取结束
            {
                printf("return 0\n");
                return 0;//读取结束
            }
            totalnum = name_msg[0] - '0';
            int count = 0;

            do
            {
                str_len = recv(sock, name_msg, 1, 0);

                name_msg[str_len] = 0;

                if (str_len == -1)//读取错误
                {
                    printf("return -1\n");
                    return -1;
                }
                if (str_len == 0)
                {
                    printf("return 0\n");
                    return 0;//读取结束
                }
                strcat(lyfstr, name_msg);
                count = str_len + count;

            } while (count < totalnum);

            lyfstr[count] = '\0';
            printf("\n");
            strcat(lyfstr, "\n");
            fputs(lyfstr, stdout);
            printf(" send message:");
            fflush(stdout);
            memset(name_msg, 0, sizeof(char));
        }
    }
    return NULL;
}*/


void SocketClient::SendToServer(void *msg)
{
    char *pMsg = (char *)msg;

    send(hSock, pMsg, strlen(pMsg), 0);
}


char *SocketClient::RecvFromServer()
{
    memset(buffer, 0, BUF_SIZE);
    recv(hSock, buffer, BUF_SIZE, 0);//读取第一个字符！获取消息的长度
    return buffer;
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
