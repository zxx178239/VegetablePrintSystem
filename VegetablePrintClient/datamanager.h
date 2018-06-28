#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "common.h"

#include <vector>
#include <string>

using namespace  std;

typedef struct VegeInfo
{
    int     vegeIndex;
    double  vegePrice;
    string  vegeName;
}VEGEINFO;


typedef struct BuyerInfo
{
    int     buyerIndex;
    string  buyerName;
    string  buyerPhonetic;
    string  buyerPhone;
    string  buyerAddr;
}BUYERINFO;


class DataManager
{
public:
    static DataManager *getInstance()
    {
        static DataManager dataManager;
        return &dataManager;
    }

private:
    DataManager(){}

public:

    void getVegeInfoFromServer();

    const vector<string>& getVegeNames();

    const QStringList& getBuyerNames();

    void saveVegeInfos(char *vegeInfo);

    void saveBuyerInfos(char *buyerInfo);

    void saveSearchInfos(char *searchInfo);


    string getBuyerPhoneticThroughName(string buyerName);

    double getVegePriceThroughName(string vegeName);

private:

    void saveVegeNames();
    void saveBuyerNames();


private:
    vector<string> vegetableNames;      // 存储的是菜名

    vector<VEGEINFO> m_vAllVegeInfos;   // 存储所有的菜品信息

    vector<BUYERINFO> m_vAllBuyerInfos; // 存储买家的所有信息

    QStringList  buyerNames;         // 暂时只用到了玩家的姓名
};

#endif // DATAMANAGER_H
