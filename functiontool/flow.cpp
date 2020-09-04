 #include "flow.h"

Flow::Flow(QObject *parent) : QObject(parent)
{
    myUdpSocket = nullptr;
    myImage = new QImage;
    myPixmap = new QPixmap;
    picBuf = new QByteArray;

    //初始化
    uPicInfo.type = -1;
    uPicInfo.size = 0;
    uPicInfo.count = 0;

    /*  Flow类 UDP模块 初始化  */
    /* IP 和 PORT <<== config.h */
    //QHostAddress ipStr("192.168.2.27");
    myUdpSocket = new QUdpSocket(this);
    quint16 port = UDP_PORT;
    //myUdpSocket->bind(ipStr,port,QAbstractSocket::DontShareAddress);
    //myUdpSocket->bind(ipStr,port,QAbstractSocket::ShareAddress);
    myUdpSocket->bind(port);
}

Flow::~Flow(){

    delete myUdpSocket;
    delete myImage;
    delete myPixmap;
    delete picBuf;

    qDebug("Flow destrcut end");
}

/* 对 UDP 套接字的开关 */
void Flow::initMyUdpSocket(bool *loop){
    this->loop = loop;

    if(*loop == true){/* 第一次绑定UDP，并开启SOCKET 可读响应信号槽 */
        this->loop = true;
        connect(myUdpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
        /* 解决 QT readyRead 信号阻塞问题 */
        if(myUdpSocket->hasPendingDatagrams()){
            emit(myUdpSocket->readyRead());
        }
    }
    else{
        /* 史上奇葩之 QUdpSocket 奇怪问题
         * 如果 QUdpSocket 有一个信号没有接收到，就不会再发出信号了,太垃圾了
         * 一、通过disconnect 再 connect 信号槽导致 QUdpSocket 收不到信号
         * 二、通过flag 来阻止 processPendingDatagram 函数响应 Qt 信号 也会导致 QUdpSocket 收不到信号
        */
        /*原因
         * Qt类接受udp套接字可读信号，
         * 经过底层过滤，发出上层 readyRead，
         * 但上一次的 readyRead 未被处理时，就不会在发送了
        */
        /* 解决办法：
         * 一：通过手动发送 myUdpSocket->readyRead() 信号，强行刷新
         * 二：重新开套接字重新绑定
        */
        this->loop =false;
        disconnect(myUdpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
        //delete myUdpSocket;
    }
}

//  /* 字节对齐首是 16 个字节 */
//  struct info{
//      int type;/* 类型: 头:0; 尾:1; */
//      long size;/* 图片总大小 */
//      int count;/* 头时为 -1, 尾时为一张图共传输了多少帧 */
//  }head,tail;

//  QByteArray *picBuf;
//  QPixmap *myPixmap;
//  struct info uPicInfo;/* 接受图片信息判断(转换)结构体 */
//  struct info *infoPtr;/* 用来接收 数组强转的 报头 */
#if 1
void Flow::processPendingDatagram(){
    //测试
    static char arr[1024*1024] = {0};/* 中间 buf：用于给picBug 追加信息，以及 接收 头尾 */
    static qint64 ret = 0;

    if(this->loop == true){

        ret = myUdpSocket->readDatagram(arr,sizeof(arr));
        myUdpSocket->readAll();
        qDebug()<<"arr = :"<<arr;
        qDebug()<<"ret = "<<ret;

        //if(ret == 16)
        if(ret == 12)
        {/* 是结构体 */
            /* 说明是 结构体, 把接收到的buf 转换成结构体 */
            //infoPtr = (struct info *)arr;
            infoPtr = reinterpret_cast<struct info *>(arr);

            /* 大端转小端 ?? 好像不需要 ?? 为什么不需要呢 */

//            for(int i = 0; i<20; i++){
//                qDebug("%d : %#x",i,arr[i]);
//            }
//            qDebug()<<"接受到的结构体";
//            qDebug()<<"infoptr.size = %ld"<<infoPtr->size;
//            qDebug()<<"infoptr.type = %d"<<infoPtr->type;
//            qDebug()<<"infoptr.count = %d"<<infoPtr->count;
//            qDebug()<<"本地结构体";
//            qDebug()<<"uPicInfo.size = %ld"<<uPicInfo.size;
//            //qDebug()<<"uPicInfo.type = %d"<<uPicInfo.type;
//            qDebug()<<"uPicInfo.count = %d"<<uPicInfo.count;

            if(infoPtr->type == 0){/*头*/
                picBuf->clear();
//                qDebug()<<"清空后 picBuf size  ="<<picBuf->size();
                infoPtr->size = uPicInfo.size;/* 保存图片大小 */
            }
            else{/*尾*//*校验正确:显示图片; 不正确丢包*/
                if(infoPtr->size == uPicInfo.size && infoPtr->count == uPicInfo.count){
                    qDebug()<<"校验：大小正确 且 计数正确";
                    myPixmap->loadFromData(*picBuf);
                    emit(sendPix(*myPixmap));
                }
                else {
                    qDebug()<<"校验：大小错误 或 计数错误";
                }
                //    /* 接受结构体初始化,开始接收一张图片前,结束一张图片结束后*/
                uPicInfo.type = -1;
                uPicInfo.size = 0;
                uPicInfo.count = 0;
            }
        }
        else{/*身子*/
            /* 接受的频率 必须大于 发送频率，否则会出现混乱 */
            /* 代码尽量精简，以提高接受的频率 */
            qDebug()<<"身子里 picBuf size  ="<<picBuf->size();
            qDebug()<<"picBuf = :"<<picBuf;
            uPicInfo.size += ret;
            uPicInfo.count++;

            picBuf->append(arr,ret);/* 数据添加进 picBuf */
            qDebug()<<"count = "<<uPicInfo.count<<"size ="<<uPicInfo.size;
        }
    }
}
#else

#endif
