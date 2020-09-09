#include "control.h"

Control::Control(QObject *parent) : QObject(parent)
{    
    memset(&myMessage,0,sizeof(myMessage));

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
        myTcpSocket->connectToHost("192.168.220.145", 6666);
        //myTcpSocket->connectToHost("192.168.2.26", 6666);

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
    qDebug()<<"myTcpSocket readyRead ";
    //QByteArray buf = myTcpSocket->readAll();
    QByteArray buf;

    if(myTcpSocket->bytesAvailable() >= qint64(sizeof(myMessage))){
        /* 读数据 */
        buf = myTcpSocket->read(sizeof(myMessage));
    }else{
        qDebug()<<"定义的结构体太大，不能一次读取完毕"<<__LINE__;
    }
    myMessage = *reinterpret_cast<struct message_user *>(buf.data());
//    myMessage.type = 0;
//    myMessage.buf = 0;
//    myMessage.nrow = 10;
//    myMessage.ncolumn = 11;
//    myMessage.sqlbuf
    qDebug()<<"--------------------"<<__LINE__;

    qDebug()<<"myMessage.buf = "<<myMessage.buf;
    qDebug()<<"myMessage.ncolumn = "<<myMessage.ncolumn;
    qDebug()<<"myMessage.nrow = "<<myMessage.nrow;
    qDebug()<<"myMessage.sqlbuf = "<<myMessage.sqlbuf;

    QString str(myMessage.buf);
    if(buf == "stateEcho"){/* 如果是 数据库查询信息 关联到 masterSelect 显示 */
        emit(echoInfo(str));
    }
    else if(str == "chargeLink"){/* 如果是 付费字符串 关联到 advertcharge 生成二维码 再关联到 advertchargevideo 显示二维码 */
        emit(echoQrencode(str));
    }
    else if(str == "comeInfo"){
        /* 如果是 车辆进入信息 就关联到 video 并显示 车牌号  欢迎光临 */
        /* 如果数据框放在 qt 上，就把还要关联数据库插入函数 */
        qDebug()<<"myMessagebuf.sqlbuf = "<<myMessage.sqlbuf;
        // emit(carIn(str));
    }
}

/*信息结构体*/
//struct __attribute__ ((__packed__)) message_user{
//    char buf[32];//消息指令结构体
//    char sqlbuf[3096];
//    int nrow;//数据库函数执行后得到的记录的数目
//    int ncolumn;//数据库函数执行得到字段的数目
//};

void Control::ctlMessage(QString str ,QString info){
    QByteArray arr;
    struct message_user ms;
    memset(&ms,0,sizeof(ms));

    /* 模板 */
    //arr = str.toUtf8();
    //arr = str.toLatin1();
    //myTcpSocket->write(arr);

    if(str == "查询状态"){
        strcpy(ms.buf,"asksql");
        qDebug()<<"____________"<<__LINE__;
        myTcpSocket->write(reinterpret_cast<char*>(&ms),sizeof(ms));
    }
    else if(str == "查询车主"){
        strcpy(ms.buf,"asksqlmaster");
        qDebug()<<"____________"<<__LINE__;
        myTcpSocket->write(reinterpret_cast<char*>(&ms),sizeof(ms));

        //arr = str.toUtf8();
        //myTcpSocket->write(arr);
    }
    else if(str == "查询车牌"){
        strcpy(ms.buf,"asksqlcarid");
        qDebug()<<"____________"<<__LINE__;
        myTcpSocket->write(reinterpret_cast<char*>(&ms),sizeof(ms));
    }
    else if(str == "抬杆"){
//        str = "raisepole\n";
//        arr = str.toUtf8();
//        myTcpSocket->write(arr);
        strcpy(ms.buf,"raisepole");
        qDebug()<<"____________"<<__LINE__;
        myTcpSocket->write(reinterpret_cast<char*>(&ms),sizeof(ms));
    }
    else if(str == "下杆"){
//        qDebug()<<"downpole";
//        str = "downpole";
//        arr = str.toUtf8();
//        myTcpSocket->write(arr);
        strcpy(ms.buf,"downpole");
        qDebug()<<"____________"<<__LINE__;
        myTcpSocket->write(reinterpret_cast<char*>(&ms),sizeof(ms));
    }
    else if(str == "打开摄像头"){
//        str = "getcaminfo";
//        arr = str.toUtf8();
//        myTcpSocket->write(arr);
        strcpy(ms.buf,"getcaminfo");
        qDebug()<<"____________"<<__LINE__;
        myTcpSocket->write(reinterpret_cast<char*>(&ms),sizeof(ms));
    }
    else if(str == "关闭摄像头"){
//        str = "stopcaminfo";
//        arr = str.toUtf8();
//        myTcpSocket->write(arr);
        strcpy(ms.buf,"stopcaminfo");
        qDebug()<<"____________"<<__LINE__;
        myTcpSocket->write(reinterpret_cast<char*>(&ms),sizeof(ms));
    }
    else if(str == "测试"){
//        str = "testEcho";
//        arr = str.toUtf8();
//        myTcpSocket->write(arr);
        strcpy(ms.buf,"testEcho");
        qDebug()<<"____________"<<__LINE__;
        myTcpSocket->write(reinterpret_cast<char*>(&ms),sizeof(ms));
    }
}

