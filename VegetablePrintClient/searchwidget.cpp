#include "searchwidget.h"
#include "ui_searchwidget.h"
#include "common.h"
#include "socketclient.h"

extern SocketClient sockClient;

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget)
{
    ui->setupUi(this);
    InitUI();
}

SearchWidget::~SearchWidget()
{
    delete ui;

}

void SearchWidget::InitUI()
{
    // 设置默认的是按姓名查找
    InitRadioBtn();

    // 绑定搜索按钮的槽函数
    connect(ui->searchBtn, SIGNAL(clicked(bool)), this, SLOT(ClickSearchBtn()));

    //InitTreeWidget();
}

void SearchWidget::InitRadioBtn()
{
    ui->nameRadioButton->setChecked(true);

    connect(ui->nameRadioButton, SIGNAL(clicked(bool)), this, SLOT(ClickRadioBtns()));
    connect(ui->timeRadioButton, SIGNAL(clicked(bool)), this, SLOT(ClickRadioBtns()));

    ui->timeWidget->hide();
    radioIndex = 0;     // 初始是0，表明选中的是按姓名查找
    InitNameComboBox();
}


void SearchWidget::InitNameComboBox()
{
    QStringList allBuyerName = DataManager::getInstance()->getBuyerNames();
    ui->nameComboBox->addItems(allBuyerName);
}

void SearchWidget::InitDateEdit()
{
    ui->dateEdit->setDate(QDate::currentDate());
}

/*
void SearchWidget::InitTreeWidget()
{


    QTreeWidgetItem *parentItem1=new QTreeWidgetItem(ui->treeWidget,QStringList("parent1"));
    QTreeWidgetItem *parentItem2=new QTreeWidgetItem(ui->treeWidget,QStringList("parent2"));

    QTreeWidgetItem *childItem1=new QTreeWidgetItem(parentItem1,QStringList("child1"));
    QTreeWidgetItem *childItem2=new QTreeWidgetItem(parentItem2,QStringList("child2"));

    QLabel *parentLable1=new QLabel("parentWindow1");
    QLabel *parentLable2=new QLabel("parentWindow2");

    QLabel *childLable1=new QLabel("childLable1");
    QLabel *childLable2=new QLabel("childLable2");

    ui->stackedWidget->addWidget(parentLable1);
    ui->stackedWidget->addWidget(parentLable2);
    ui->stackedWidget->addWidget(childLable1);
    ui->stackedWidget->addWidget(childLable2);
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(ClickChange(QTreeWidgetItem*, int)));
}


void SearchWidget::ClickChange(QTreeWidgetItem *item1, int count1)
{
    QTreeWidgetItem *item=item1;

    if(item->text(count1)=="parent1")
    {
        ui->stackedWidget->setCurrentIndex(0);
    }

    else if(item->text(count1)=="parent2")
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(item->text(count1)=="child1")
    {

        ui->stackedWidget->setCurrentIndex(2);

    }
    else if(item->text(count1)=="child2")
    {

        ui->stackedWidget->setCurrentIndex(3);

    }

}
*/

void SearchWidget::ClickRadioBtns()
{
    if(QObject::sender()->objectName() == "nameRadioButton")
    {
        ui->nameWidget->show();
        ui->timeWidget->hide();
        radioIndex = 0;
    }else if(QObject::sender()->objectName() == "timeRadioButton")
    {
        ui->nameWidget->hide();
        ui->timeWidget->show();
        InitDateEdit();
        radioIndex = 1;
    }

}

void SearchWidget::ClickSearchBtn()
{
    char searchBuf[100] = "";
    if(radioIndex == 0)
    {
        cout << "search through name" << endl;
        SearchThroughName(searchBuf);
    }else if(radioIndex == 1)
    {
        cout << "search through date" << endl;
        SearchThroughDate(searchBuf);
    }
    SendSearchSentenceToServer(searchBuf);
}

void SearchWidget::SearchThroughName(char *buf)
{
    string searchName = string(ui->nameComboBox->currentText().toLocal8Bit());
    string searchNamePhonetic = DataManager::getInstance()->getBuyerPhoneticThroughName(searchName.c_str());
    sprintf(buf, "%d,%d,%s", REQUEST_SEARCH, radioIndex, searchNamePhonetic.c_str());
}

void SearchWidget::SearchThroughDate(char *buf)
{
    string dateTime = string(ui->dateEdit->date().toString("yyyy-MM-dd").toLocal8Bit());

    sprintf(buf, "%d,%d,%s", REQUEST_SEARCH, radioIndex, dateTime.c_str());
}


void SearchWidget::SendSearchSentenceToServer(char *str)
{
    sockClient.PushToSendQueue(str);
}
