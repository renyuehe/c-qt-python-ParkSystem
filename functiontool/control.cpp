#include "control.h"

Control::Control(QObject *parent) : QObject(parent)
{    
    //1. 创建TCP套接字对象
    myTcpSocket = new QTcpSocket(this);
    //2. 已连接、数据可读、失败信号连接、断开链接
    connect(myTcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(myTcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    /*
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(m_pTcpSocket, static_cast<QAbstractSocketErrorSignal>(&QTcpSocket::error), this, &SimpleTcpSocketClientDemo::error);
    */
    connect(myTcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    qDebug()<<"Crontrol   construct  ok !!!";
}

Control::~Control(){
    qDebug("Control destrcut end");
}

void Control::myBtnConnect(bool isConnect){
    if(isConnect == true){
        //3. 与服务器端建立连接
        qDebug()<<"tcp try connectToHost";
        myTcpSocket->connectToHost("192.168.43.43", 6666);

        if(myTcpSocket->waitForConnected(5000)){
            qDebug("myTcpSocket Connected!");
        }
        else {
            emit(connectOvertime());
        }
        //4. 同步处理-等待数据可读，
        // 我不需要读数据，我只需要发数据后再读数据就行了 所以屏蔽了
        //myTcpSocket->waitForReadyRead();
    }else if(isConnect == false){
        qDebug()<<"tcp disconnected end";
        myTcpSocket->disconnectFromHost();
        /* 可能再这一条语句间 就已经断开链接 了, 所以就不存在等待断开连接的说法了 */
        if(myTcpSocket->state() == QAbstractSocket::UnconnectedState ||myTcpSocket->waitForDisconnected(5000)){
            qDebug("myTcpSocket disConnected!");
        }
        else {
            emit(connectOvertime());
        }
    }
}

void Control::connected(){
    qDebug()<<"myTcpSocket connected successful ";
    emit(tcpConnectOk());
}

void Control::disconnected(){
    qDebug()<<"myTcpSocket disconnected successful ";
    emit(tcpDisconnectOk());
}

void Control::readyRead(){
    qDebug()<<"myTcpSocket connected readyRead ";
    QByteArray buf = myTcpSocket->readAll();
    QString str(buf);
    emit(echoInfo(str));
}

void Control::ctlMessage(QString str ,QString info){
    QByteArray arr;
    struct select sc;
    memset(&sc,0,sizeof(sc));

    /* 模板 */
    //arr = str.toUtf8();
    //arr = str.toLatin1();
    //myTcpSocket->write(arr);

    if(str == "查询车主"){
        strcpy(sc.table_name,"t_status");
        strcpy(sc.column,"master_name");
        strcpy(sc.values,info.toUtf8());
        qDebug()<<sc.column<<":"<<sc.table_name<<":"<<sc.values;
        myTcpSocket->write(reinterpret_cast<char*>(&sc),sizeof(sc));

        //arr = str.toUtf8();
        //myTcpSocket->write(arr);
    }
    if(str == "查询车牌"){
        strcpy(sc.table_name,"t_status");
        strcpy(sc.column,"plate_number");
        strcpy(sc.values,info.toUtf8());
        qDebug()<<sc.column<<":"<<sc.table_name<<":"<<sc.values;
        myTcpSocket->write(reinterpret_cast<char*>(&sc),sizeof(sc));
    }
    else if(str == "抬杆"){
        str = "raisepole\n";
        arr = str.toUtf8();
        myTcpSocket->write(arr);
    }
    else if(str == "下杆"){
        qDebug()<<"downpole";
        str = "downpole";
        arr = str.toUtf8();
        myTcpSocket->write(arr);
    }
    else if(str == "打开摄像头"){
        str = "getcaminfo";
        arr = str.toUtf8();
        myTcpSocket->write(arr);
    }
    else if(str == "关闭摄像头"){
        str = "stopcaminfo";
        arr = str.toUtf8();
        myTcpSocket->write(arr);
    }
}

