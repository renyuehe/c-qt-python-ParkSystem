#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QTcpSocket>
#include <QtEndian>
#include "config.h"

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);
    ~Control();

signals:
    void tcpConnectOk();
    void tcpDisconnectOk();
    void connectOvertime();
    /* 套接字可读 返回的信息 */
    void echoInfo(QString str);
    void echoQrencode(QString str);

private slots:
    void connected();
    void readyRead();
    void disconnected();
    void myBtnConnect(bool isConnect);
    void ctlMessage(QString,QString);

private:
    /* 控制套接字 */
    QTcpSocket *myTcpSocket;

    struct message_user myMessage;
};

#endif // CONTROL_H
