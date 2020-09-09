#include "advertchargevideo.h"
#include "ui_advertchargevideo.h"


AdvertChargeVideo::AdvertChargeVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::charge)
{
    ui->setupUi(this);

    /* 设置 lable 中间一半 */
    //ui->label->setGeometry(this->width()/4,this->height()/4,this->width()/2,this->height()/2);
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
{
    ui->label_top->clear();
    ui->label->clear();
    //
    static char buf[2];
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
void AdvertChargeVideo::codeShow(QPixmap codePix){
#if 0
    codePix =codePix.scaled(this->geometry().width(),this->geometry().height(),Qt::IgnoreAspectRatio);

    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(codePix));
    this->setPalette(palette);// 给widget加上背景图

#else
    QPalette palette;
    palette.setColor(QPalette::Window,QColor("white"));
    this->setPalette(palette);// 给widget加上背景图

//    picStr = "扫二维码付钱";
//    //字符串 转换成 图片就可以显示了，还可以滑动！！！
//    ui->label_top->setPixmap(chargePix.scaled(ui->label_top->width(),ui->label_top->height()));

    ui->label_top->setText("扫码付款");
    ui->label->setPixmap(codePix.scaled(ui->label->width(),ui->label->height(),Qt::IgnoreAspectRatio));
#endif
}
