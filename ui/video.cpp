#include "ui/video.h"
#include "ui_video.h"

Video::Video(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Video)
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

Video::~Video()
{
    delete font;
    delete pa;
    delete ui;

    qDebug("video destrcut end");
}

/* 重写事件 */
void Video::closeEvent(QCloseEvent *e){
    /* 事件未被处理,会向上传递 */
    //e->accept();
    e->ignore();//?????区别

    hide();/*隐藏窗口*/
}

void Video::timeDisplay(QString str){

    ui->label_time->setText(str);
    //update();
}

void Video::picDisplay(QPixmap pixmap){
    /* frameGeometry 是外边框， geometry 是内边框 */
    //pixmap = pixmap.scaled(this->frameGeometry().width(),this->frameGeometry().height(),Qt::IgnoreAspectRatio);
    //pixmap = pixmap.scaled(this->geometry().width(),this->geometry().height(),Qt::IgnoreAspectRatio);
    pixmap = pixmap.scaled(this->geometry().width(),this->geometry().height(),Qt::IgnoreAspectRatio);

    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(pixmap));
    this->setPalette(palette);// 给widget加上背景图
}
