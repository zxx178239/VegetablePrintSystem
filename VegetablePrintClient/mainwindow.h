#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "vegetableprintwidget.h"
#include "searchwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    VegetablePrintWidget *vegetablePrintWidget;

    SearchWidget         *searchWidget;



    void initUI();
public slots:
    void ClickTabChange(int );
};

#endif // MAINWINDOW_H
