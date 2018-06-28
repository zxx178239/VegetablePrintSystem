#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include "socketclient.h"
#include "common.h"

extern SocketClient sockClient;

class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 90; // 设置每个tabBar中item的大小
            s.rheight() = 44;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                }
                else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 这里向服务端请求vegetable信息
    char *vegeInfo = sockClient.RecvFromServer();

    DataManager::getInstance()->saveVegeInfos(vegeInfo);

    // 向服务端请求 买家信息
    char tmpBuf[60] = "";
    itoa(REQUEST_BUYERINFO, tmpBuf, 10);
    sockClient.SendToServer((void *)tmpBuf);
    char *buyerInfo = sockClient.RecvFromServer();
    DataManager::getInstance()->saveBuyerInfos(buyerInfo);

    /*
    char x[] = "hello!\n";
    SocketClient::getInstance()->SendToServer((void *)x);
    SocketClient::getInstance()->RecvFromServer(NULL);
    Sleep(1000);
    SocketClient::getInstance()->SendToServer((void *)x);
    SocketClient::getInstance()->RecvFromServer(NULL);
    Sleep(1000);
    SocketClient::getInstance()->SendToServer((void *)x);
    SocketClient::getInstance()->RecvFromServer(NULL);
    */

    // 初始化UI
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete vegetablePrintWidget;
    vegetablePrintWidget = NULL;

    delete searchWidget;
    searchWidget = NULL;

    sockClient.CloseSock();
}

void MainWindow::initUI()
{
    ui->mainTabWidget->setTabPosition(QTabWidget::West);
    ui->mainTabWidget->tabBar()->setStyle(new CustomTabStyle);

    ui->mainTabWidget->removeTab(0);
    ui->mainTabWidget->removeTab(0);

    vegetablePrintWidget = new VegetablePrintWidget();
    ui->mainTabWidget->addTab(vegetablePrintWidget, "菜单打印");

    searchWidget = new SearchWidget();
    ui->mainTabWidget->addTab(searchWidget, "查找系统");

    // 切换标签页的函数
    //connect(ui->mainTabWidget, SIGNAL(currentChanged(int)), this, SLOT(ClickTabChange(int)));
}


void MainWindow::ClickTabChange(int pageIndex)
{

}
