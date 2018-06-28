#include "mainwindow.h"
#include <QApplication>
#include "socketclient.h"

SocketClient sockClient;

int main(int argc, char *argv[])
{
    // 1. 连接服务器
    sockClient.Initialize();

    // 2. 启动界面
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
