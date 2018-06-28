#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <string>

#include <QMessageBox>
#include <QStringList>
#include <QMenu>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QPrintDialog>
#include <QDateTime>
#include <cstdlib>
#include <cstdio>
#include <QDebug>
#include "messagedefine.h"
#include "datamanager.h"

using namespace std;


const int ROW_ELEMENTS = 5;

const string TABLEVIEWHEAD[] = {"菜名", "单价",  "重量 (斤)", "价格 (元)", "备注"};
const int TABLEVIEWHEAD_COUNT = 5;

const int BUF_SIZE = 65535;

#endif // COMMON_H
