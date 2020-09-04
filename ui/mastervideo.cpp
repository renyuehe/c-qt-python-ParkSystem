#include "mastervideo.h"
#include "ui_mastervideo.h"

MasterVideo::MasterVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MasterVideo)
{
    ui->setupUi(this);

    font = new QFont;
    pa = new QPalette;
    font->setFamily("黑体");
    font->setBold(true);
    font->setPointSize(20);
    pa->setColor(QPalette::WindowText,Qt::red);
    ui->label_time->setPalette(*pa);
    ui->label_time->setFont(*font);
}

MasterVideo::~MasterVideo()
{
    delete font;
    delete pa;
    delete ui;

    qDebug("master video destrcut end");
}

/* 显示时间 */
void MasterVideo::timeDisplay(QString str){
    ui->label_time->setText(str);
}

/* 显示画面 */
void MasterVideo::picDisplay(QPixmap pixmap){
    /* frameGeometry 是外边框， geometry 是内边框 */
    //pixmap = pixmap.scaled(this->frameGeometry().width(),this->frameGeometry().height(),Qt::IgnoreAspectRatio);
    //pixmap = pixmap.scaled(ui->label_video->width(),ui->label_video->height(),Qt::IgnoreAspectRatio);
    pixmap = pixmap.scaled(this->geometry().width(),this->geometry().height(),Qt::IgnoreAspectRatio);

    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(pixmap));
    this->setPalette(palette);// 给widget加上背景图
}
