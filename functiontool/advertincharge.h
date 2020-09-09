#ifndef ADVERTINCHARGE_H
#define ADVERTINCHARGE_H

#include <QObject>
#include <QPixmap>
#include "qrencode/qrencode.h"


class AdvertInCharge: public QObject
{
    Q_OBJECT
public:
    explicit AdvertInCharge(QObject *parent = nullptr);
    ~AdvertInCharge();

    /*
     * @brief GernerateQRCode
     * 生成二维码函数
     * @param text  二维码内容
     * @param qrPixmap  二维码像素图
     * @param scale 二维码缩放比例
     */
    void GernerateQRCode(const QString &text, QPixmap &qrPixmap, int scale);

public slots:
    void advertRun();


signals:
    /* 广告图第 i 个 信号 */
    void sendPicNumToChargeAdvertVedio(int);
    /* 二维码生成 图 */
    void sendCodePix(QPixmap);

public:
    /* 动态二维码 退出线程 */
    bool flagRun = 0;/* 0 静态, 1 动态 */
    bool isAdervt = 0;/* 1 广告, 0 二维码 */

    QString codeStr;/* 主次线程共享 字符串 */
};

#endif // ADVERTINCHARGE_H
