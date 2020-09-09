#include "advertincharge.h"
#include <QDebug>
#include <QtWidgets>

AdvertInCharge::AdvertInCharge(QObject *parent):QObject (parent)
{

}

AdvertInCharge::~AdvertInCharge(){
    qDebug("AdvertInCharge destrcut end");
}

void AdvertInCharge::advertRun(){
    int i = 0;
    while(1){
        if(flagRun == 1 && isAdervt == 1){
            qDebug()<<"执行广告"<<endl;
            emit(sendPicNumToChargeAdvertVedio(i));
            Sleep(100);
        }
        else if(flagRun == 0 && isAdervt == 1){
            qDebug()<<"静态广告"<<endl;
            Sleep(100);
        }
        else if(flagRun == 0 && isAdervt == 0){
            qDebug()<<"二维码"<<endl;
            QPixmap codePix;
            GernerateQRCode(codeStr,codePix,1);//生成二维码
            emit(sendCodePix(codePix));
            Sleep(100);
        }
        else if(flagRun == 1 && isAdervt == 0){
            qDebug()<<"动态二维码,退出线程"<<endl;
            break;/* 退出线程 */
        }
        i>16? i=0:i++;
    }
    /* 注意线程函数无法返回的话,则无法释放,除了terminate杀死线程,还有什么办法可以终止线程不异常 */
}

/*
 * @brief GernerateQRCode
 * 生成二维码函数
 * @param text  二维码内容
 * @param qrPixmap  二维码像素图
 * @param scale 二维码缩放比例
 */
void AdvertInCharge::GernerateQRCode(const QString &text, QPixmap &qrPixmap, int scale)
{
     qDebug()<<"_____________"<<__LINE__;
     if(text.isEmpty()){
         return;
     }

     //二维码数据
     QRcode *qrCode = nullptr;

     //这里二维码版本传入参数是2,实际上二维码生成后，它的版本是根据二维码内容来决定的
     qrCode = QRcode_encodeString(text.toStdString().c_str(), scale,
                                  QR_ECLEVEL_Q, QR_MODE_8, 1);

     if(nullptr == qrCode){
         return;
     }
     qDebug()<<"_____________"<<__LINE__;
     int qrCode_Width = qrCode->width > 0 ? qrCode->width : 1;
     int width = scale * qrCode_Width;
     int height = scale * qrCode_Width;

     QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
    qDebug()<<"_____________"<<__LINE__;
     QPainter painter(&image);
     QColor background(Qt::white);
     painter.setBrush(background);
     painter.setPen(Qt::NoPen);
     painter.drawRect(0, 0, width, height);
     QColor foreground(Qt::black);
     painter.setBrush(foreground);
     qDebug()<<"_____________"<<__LINE__;
     for(int y = 0; y < qrCode_Width; ++y)
     {
         for(int x = 0; x < qrCode_Width; ++x)
         {
             unsigned char character = qrCode->data[y * qrCode_Width + x];
             if(character & 0x01)
             {
                QRect rect(x * scale, y * scale, scale, scale);
                painter.drawRects(&rect, 1);
             }
         }
     }
    qDebug()<<"_____________"<<__LINE__;
     qrPixmap = QPixmap::fromImage(image);
     QRcode_free(qrCode);
}
