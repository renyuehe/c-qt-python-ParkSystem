#ifndef ADVERTINCHARGE_H
#define ADVERTINCHARGE_H

#include <QObject>

class AdvertInCharge: public QObject
{
    Q_OBJECT
public:
    explicit AdvertInCharge(QObject *parent = nullptr);
    ~AdvertInCharge();

public slots:
    void advertRun();

signals:
    void sendPicNumToChargeAdvertVedio(int);

public:
    /* 动态二维码 退出线程 */
    bool flagRun = 0;/* 0 静态, 1 动态 */
    bool isAdervt = 0;/* 1 广告, 0 二维码 */

    QPixmap *twoDimensionCode; /* 二维码 主次线程共享变量 , 只需要在主进程中改变内容即可 */
};

#endif // ADVERTINCHARGE_H
