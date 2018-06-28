#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "common.h"


namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = 0);
    ~SearchWidget();

private:
    Ui::SearchWidget *ui;

private:
    void InitUI();

    void InitRadioBtn();

    void InitNameComboBox();

    void InitDateEdit();

    void InitTreeWidget();


    void SearchThroughName(char *buf);

    void SearchThroughDate(char *buf);

    void SendSearchSentenceToServer(char *buf);
private:
    int radioIndex;

public slots:
    void ClickRadioBtns();

    void ClickSearchBtn();

    //void ClickChange(QTreeWidgetItem *item1, int count);
};

#endif // SEARCHWIDGET_H
