#include "advertchargevideo.h"
#include "ui_advertchargevideo.h"


AdvertChargeVideo::AdvertChargeVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::charge)
{
    ui->setupUi(this);
}

AdvertChargeVideo::~AdvertChargeVideo()
{
    delete ui;
    qDebug("advertChargeVideo destrcut end");
}

/* 重写事件 */
void AdvertChargeVideo::closeEvent(QCloseEvent *e){
    e->ignore();//?????区别
    hide();/*隐藏窗口*/
}

void AdvertChargeVideo::picShow(int i)
{   static char buf[2];
    itoa(i,buf,10);
    picStr = ":/advertisement";
    picStr.append(buf).append(".jpg");
    qDebug()<<picStr;

    chargePix.load(picStr);
    chargePix =chargePix.scaled(this->geometry().width(),this->geometry().height(),Qt::IgnoreAspectRatio);

    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(chargePix));
    this->setPalette(palette);// 给widget加上背景图
}
