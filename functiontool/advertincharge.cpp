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
            Sleep(200);
        }
        else if(flagRun == 0 && isAdervt == 1){
            qDebug()<<"静态广告"<<endl;
            Sleep(1000);
        }
        else if(flagRun == 0 && isAdervt == 0){
            qDebug()<<"二维码"<<endl;
            Sleep(1000);
        }
        else if(flagRun == 1 && isAdervt == 0){
            qDebug()<<"动态二维码,退出线程"<<endl;
            break;/* 退出线程 */
        }
        i>16? i=0:i++;
    }
    /* 注意线程函数无法返回的话,则无法释放,除了terminate杀死线程,还有什么办法可以终止线程不异常 */
}
