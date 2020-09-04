#include "timer.h"

Timer::Timer(QObject *parent) : QObject(parent)
{

}
Timer::~Timer(){
    qDebug("timer destrcut end");
}

/* 获取系统并发送给Video框架 */
void Timer::timerRun(bool *loopTimer){

    qDebug()<<"------------------timerRun run-------------------";
    while(*loopTimer && flagRun==1){
        QDateTime dateTime(QDateTime::currentDateTime());
        QString dateStr(dateTime.toString("yyyy-MM-dd hh:mm::ss ddd"));

        emit(sendTimeToVideo(dateStr));
        Sleep(1000);
    }
}

