#ifndef FLOW_H
#define FLOW_H

#include <stdio.h>
#include <QtWidgets>
#include <QObject>
#include <QUdpSocket>
#include <QPixmap>
#include <QImage>
#include <QtEndian>
#include <QString>
#include "config.h"

class Flow : public QObject
{
    Q_OBJECT
public:
    explicit Flow(QObject *parent = nullptr);
    ~Flow();

signals:
    void sendPix(QPixmap pixmap);
public slots:
    void initMyUdpSocket(bool *loop);
    void processPendingDatagram();

private:
    bool loop;
    QUdpSocket *myUdpSocket;
    QByteArray *picBuf;
    QPixmap *myPixmap;
    QImage *myImage;

    struct info uPicInfo;/* 接受图片信息判断(转换)结构体 */
    struct info *infoPtr;/* 用来接收 数组强转的 报头 */
};

#endif // FLOW_H
