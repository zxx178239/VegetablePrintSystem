#include "datamanager.h"


const vector<string>& DataManager::getVegeNames()
{
    return vegetableNames;
}


const QStringList& DataManager::getBuyerNames()
{
    return buyerNames;
}

void DataManager::saveVegeNames()
{
    for(int index = 0; index < m_vAllVegeInfos.size(); ++ index)
    {
        vegetableNames.push_back(m_vAllVegeInfos[index].vegeName);
    }
}

void DataManager::saveBuyerNames()
{
    for(int index = 0; index < m_vAllBuyerInfos.size(); ++ index)
    {
        buyerNames.append(QString::fromLocal8Bit(m_vAllBuyerInfos[index].buyerName.data()));
    }
}

void DataManager::saveVegeInfos(char *vegeInfo)
{
    QString curStr;
    curStr = QString::fromLocal8Bit(vegeInfo);
    //qDebug() << curStr.toStdString().c_str() << endl;
    QStringList allVegeInfos = curStr.split('\n');

    for(int i = 0;  i < allVegeInfos.size(); ++ i)
    {
        if(allVegeInfos.at(i) == "")
            continue;
        QStringList curVegeInfo = allVegeInfos.at(i).split(',');
        VEGEINFO newInfo;

        newInfo.vegeName = string(curVegeInfo.at(0).toLocal8Bit());
        newInfo.vegePrice = curVegeInfo.at(1).toDouble();

        //cout <<  newInfo.vegeName << " " << newInfo.vegePrice << endl;

        m_vAllVegeInfos.push_back(newInfo);
    }
    saveVegeNames();
}

void DataManager::saveBuyerInfos(char *buyerInfo)
{
    QString curStr;
    curStr = QString::fromLocal8Bit(buyerInfo);
    qDebug() << curStr.toStdString().c_str() << endl;
    QStringList allBuyerInfos = curStr.split('\n');

    for(int i = 0;  i < allBuyerInfos.size(); ++ i)
    {
        if(allBuyerInfos.at(i) == "")
            continue;
        QStringList curVegeInfo = allBuyerInfos.at(i).split(',');
        BUYERINFO newInfo;

        newInfo.buyerIndex      = curVegeInfo.at(0).toInt();
        newInfo.buyerName       = string(curVegeInfo.at(1).toLocal8Bit());
        newInfo.buyerPhonetic   = curVegeInfo.at(2).toStdString();
        newInfo.buyerPhone      = curVegeInfo.at(3).toStdString();
        newInfo.buyerAddr       = string(curVegeInfo.at(4).toLocal8Bit());

        cout << newInfo.buyerIndex << " " << newInfo.buyerName << " " << newInfo.buyerPhonetic << endl;

        m_vAllBuyerInfos.push_back(newInfo);
    }
    saveBuyerNames();
}

string DataManager::getBuyerPhoneticThroughName(string buyerName)
{
    for(int index = 0; index < m_vAllBuyerInfos.size(); ++ index)
    {
        if(m_vAllBuyerInfos[index].buyerName == buyerName)
            return m_vAllBuyerInfos[index].buyerPhonetic;
    }
    return "";
}

// 比较low的写法啦，肯定是要写成key, val查找的形式，懒得写了
double DataManager::getVegePriceThroughName(string vegeName)
{
    for(int index = 0; index < m_vAllVegeInfos.size(); ++ index)
    {
        if(m_vAllVegeInfos[index].vegeName == vegeName)
            return m_vAllVegeInfos[index].vegePrice;
    }
    return 0.0;
}
