#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
#include <QDesktopWidget>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QDesktopWidget* desktop = QApplication::desktop();
    /*
     * In the illustration above,
     * Application One's primary screen is screen 0,
     * and App Two's primary screen is screen 1.
     * */
    qDebug()<<"这是: "<<desktop->screenNumber()<<" 号显示器";
    qDebug()<<"分辨率: "<<desktop->screenGeometry(this).width()<<desktop->screenGeometry(this).height();


    ui->label_brand->setPixmap(QPixmap(":/car.png").scaled(ui->label_brand->width(),ui->label_brand->height()));

    /* 设置系统图标 */
    mySystemTrayIcon = new QSystemTrayIcon(this);
    mySystemTrayIcon->setToolTip("智能停车系统\n朝江人羊皮");  /*设置提示信息*/
    mySystemTrayIcon->setIcon(QIcon(":/car.png"));          /* 设置图标 */
    mySystemTrayIcon->show();

        /* 把系统图标的注入菜单menu能力 和菜单弹出动作action */
    myMenu = new QMenu(this);
    myAction = new QAction(myMenu);             /*设置父对象*/
    myAction->setIcon(QIcon(":/car.png"));
    myAction->setText("退出智能停车系统");
    myMenu->addAction(myAction);                /*添加动作*/
    mySystemTrayIcon->setContextMenu(myMenu);/* 系统图标注入菜单能力 */

        /* 关联 系统图标activeted信号(双击 或 其他) */
    connect(mySystemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(slotActivated(QSystemTrayIcon::ActivationReason)));
        /* 关联 系统图标菜单动作 */
    /* 这个函数最好转到一个 中间槽(判断)进程状态，并释放没有释放的资源，再关闭也不迟 */
#if 0
    connect(myAction,SIGNAL(triggered()),this,SLOT(close()));
#else
    connect(myAction,SIGNAL(triggered()),this,SLOT(quit()));
    connect(this,SIGNAL(kill()),qApp,SLOT(quit()));
#endif

    /* 逻辑 flag */
    loopFlow = false;
    loopTimer = false;
    isConnect = false;

    /* 线程类 和 线程 condition */
    myThrFlow = new MyThr(this);
    myThrTimer = new MyThr(this);
    myThrControl = new MyThr(this);
    myThrChargeAdvert = new MyThr(this);

    /* 对象 */
    myFlow = new Flow;
    myTimer = new Timer;
    myChargeAdvert = new AdvertInCharge;
    myControl = new Control;
    ui->pushButton_connect->setEnabled(false);

    myVideo = new Video;
    myChargeAdvertVideo = new AdvertChargeVideo;

    myMasterLogin = new MasterLogin(this);/* stackWidget 对象 */
    myMasterVideo = new MasterVideo(this);
    myMasterSelect = new MasterSelect(this);
    ui->stackedWidget->addWidget(myMasterLogin);/* 添加对象进 stackedWidget */
    ui->stackedWidget->addWidget(myMasterVideo);
    ui->stackedWidget->addWidget(myMasterSelect);

    /* tcp 控制流类 */
    myControl->moveToThread(myThrControl);
    myThrControl->start();
    ui->pushButton_connect->setEnabled(true);

    /* 视频流类 */
    myFlow->moveToThread(myThrFlow);
    myThrFlow->start();

    connect(this,SIGNAL(flowStart(bool*)),myFlow,SLOT(initMyUdpSocket(bool*)));
    connect(myFlow,SIGNAL(sendPix(QPixmap)),myVideo,SLOT(picDisplay(QPixmap)));
    connect(myFlow,SIGNAL(sendPix(QPixmap)),myMasterVideo,SLOT(picDisplay(QPixmap)));

    /* Timer流类 */
    myTimer->moveToThread(myThrTimer);
    myThrTimer->start();

    connect(this,SIGNAL(timerStart(bool*)),myTimer,SLOT(timerRun(bool*)));/* 为什么传引用不行 */
    connect(myTimer,SIGNAL(sendTimeToVideo(QString)),myVideo,SLOT(timeDisplay(QString)));
    connect(myTimer,SIGNAL(sendTimeToVideo(QString)),myMasterVideo,SLOT(timeDisplay(QString)));

    /* 广告 和 二维码 类 */
    myChargeAdvert->moveToThread(myThrChargeAdvert);
    myThrChargeAdvert->start();
    connect(this,SIGNAL(advertStart()),myChargeAdvert,SLOT(advertRun()));
    connect(myChargeAdvert,SIGNAL(sendPicNumToChargeAdvertVedio(int)),myChargeAdvertVideo,SLOT(picShow(int)));


    /* Control 类 */
/* 为什么在  Lambda 表达式中 ui->pushButton_connect->setEnabled(true); 就卡死了？？
    connect(myControl, &Control::tcpConnectOk,
            [=](){
                ui->pushButton_connect->setText("断开");
                //为什么这一句会卡死
                ui->pushButton_connect->setEnabled(true);
            });

    connect(myControl, &Control::tcpDisconnectOk,
            [=](){
                ui->pushButton_connect->setText("连接");
                //为什么这一句会卡死
                ui->pushButton_connect->setEnabled(true);
            });
*/

    connect(myControl,SIGNAL(connectOvertime()),this,SLOT(connectOvertime()));
    connect(myControl,SIGNAL(tcpConnectOk()),this,SLOT(setBtnDisconnectEnable()));
    connect(myControl,SIGNAL(tcpDisconnectOk()),this,SLOT(setBtnConnectEnable()));

    connect(this,SIGNAL(connectTcp(bool)),myControl,SLOT(myBtnConnect(bool)));

    /* 发送tcp控制信号 */
    connect(myMasterSelect,SIGNAL(searchInfo(QString,QString)),myControl,SLOT(ctlMessage(QString,QString)));
    connect(this,SIGNAL(poleInfo(QString,QString)),myControl,SLOT(ctlMessage(QString,QString)));

    /* 返回套接字 可读 信息 */
    connect(myControl, SIGNAL(echoInfo(QString)),myMasterSelect, SLOT(showInfo(QString)));

    //void tcpConnectOk();
    //void tcpDisconnectOk();
}

Widget::~Widget()
{
    qDebug()<<"widget quit"<<__LINE__;
    delete myChargeAdvertVideo;
    delete myVideo;
    qDebug()<<"widget quit"<<__LINE__;
    delete ui;
    qDebug()<<"widget quit"<<__LINE__;
}

/* 重写事件 */
void Widget::closeEvent(QCloseEvent *e){
    /* 事件未被处理,会向上传递 */
    //e->accept();
    e->ignore();//?????区别

    hide();/*隐藏窗口*/
}
/* ********************************** 事件 ****************************************/
/* 重写绘图事件:做到追加绘效果 */
void Widget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
}

/************************************ 槽函数 **************************************/

/* 任务栏右键退出 回收线程资源 退出线程函数 中间站 */
void Widget::quit(){

    qDebug()<<"自定义quit"<<__LINE__;

    myChargeAdvert->deleteLater();
    myControl->deleteLater();
    myTimer->deleteLater();
    //myFlow->deleteLater();
    qDebug()<<"自定义quit"<<__LINE__;

    myTimer->flagRun = 0;
    myThrTimer->exit(0);
    //myThrTimer->quit();
    myThrTimer->wait();

    myThrFlow->exit(0);
    //myThrFlow->quit();
    myThrFlow->wait();

    /* 约定:动态二维码则退出 线程循环函数 */
    myChargeAdvert->flagRun = 1;
    myChargeAdvert->isAdervt = 0;
    myThrChargeAdvert->exit(0);/* 等待下成函数退出 */
    //myThrChargeAdvert->quit();
    myThrChargeAdvert->wait();

    myThrControl->exit(0);/* 控制流 必须执行完毕才能退出 */
    //myThrControl->quit();
    myThrControl->wait();
    qDebug()<<"自定义quit"<<__LINE__;

    emit(kill());
    //qApp->quit();
    qDebug()<<"自定义quit"<<__LINE__;
}

/* 响应任务栏图标 activated 信号 */
void Widget::slotActivated(QSystemTrayIcon::ActivationReason reason){
    if(reason == QSystemTrayIcon::DoubleClick){
        showNormal();
    }
}

/* 开始运行 */
void Widget::on_pushButton_start_clicked()
{
    if(ui->pushButton_start->text() == "运行"){
        ui->pushButton_start->setText("停止");
        loopTimer = true;
        myVideo->show();
        myChargeAdvertVideo->show();

        /* 开始广告_二维码 *//* 只有线程类 run 中的局部变量才是线程自己的变量, 主线程的对象中的普通变量是 线程和主线程共享变量 */
        myChargeAdvert->flagRun = 1;
        myChargeAdvert->isAdervt = 1;
        emit(advertStart());
        emit(timerStart(&loopTimer));
        loopFlow = true;
        emit(flowStart(&loopFlow));
    }
    else if(ui->pushButton_start->text() == "停止"){
        ui->pushButton_start->setText("运行");
        /* 停止time */
        loopTimer = false;
        /* 停止视频 */
        loopFlow = false;
        /* 停止广告 */
        myChargeAdvert->flagRun = 0;

        emit(flowStart(&loopFlow));

        /* 停止运行逻辑 */
        //
        //
    }
}

/* 使用 clicked bool 之前需要先把 ui 中选中 clickedable */


/* 连接 TCP 服务器 */
void Widget::on_pushButton_connect_clicked()
{
    if(ui->pushButton_connect->text() == "连接")
    {
        isConnect = true;
        emit(connectTcp(isConnect));
        ui->pushButton_connect->setEnabled(false);
        /* pushButton显示 断开 应该有信号槽来实现改连接的字 */
    }
    else if(ui->pushButton_connect->text() == "断开"){
        isConnect = false;
        emit(connectTcp(isConnect));
        ui->pushButton_connect->setEnabled(false);
        /* pushButton显示 连接 应该有信号槽来实现改连接的字 */
    }
}

void Widget::connectOvertime(){
    ui->pushButton_connect->setEnabled(true);

    //返回值就是按钮组点击的某个按钮【关闭和QMessageBox::Close、QMessageBox::No，...一致】
    QMessageBox::StandardButton btn = QMessageBox::warning(nullptr,//没有父对象
                         "超时操作",
                         "检查网络,并再次尝试",
                         QMessageBox::Ok
                         );
    if(QMessageBox::Ok == btn){
        qDebug("QMessageBox::Ok ==========");
    }
    qDebug("超时了");
}

void Widget::setBtnDisconnectEnable(){
    ui->pushButton_connect->setText("断开");
    ui->pushButton_connect->setEnabled(true);
}
void Widget::setBtnConnectEnable(){
    ui->pushButton_connect->setText("连接");
    ui->pushButton_connect->setEnabled(true);
}

/* 设置当前 socketWidget 界面 */
void Widget::on_pushButton_video_clicked()
{
    ui->stackedWidget->setCurrentWidget(myMasterVideo);
}
void Widget::on_pushButton_login_clicked()
{
    ui->stackedWidget->setCurrentWidget(myMasterLogin);
}
void Widget::on_pushButton_select_clicked()
{
    ui->stackedWidget->setCurrentWidget(myMasterSelect);
}

void Widget::on_pushButton_pole_clicked()
{
    if(ui->pushButton_pole->text() == "抬杆"){
        emit(poleInfo("抬杆","0"));
        ui->pushButton_pole->setText("下杆");
    }else if(ui->pushButton_pole->text() == "下杆"){
        emit(poleInfo("下杆","0"));
        ui->pushButton_pole->setText("抬杆");
    }
}

void Widget::on_pushButton_camCtl_clicked()
{
    if(ui->pushButton_camCtl->text() == "打开摄像头"){
        emit(poleInfo("打开摄像头","0"));
        ui->pushButton_camCtl->setText("关闭摄像头");
    }else if(ui->pushButton_camCtl->text() == "关闭摄像头"){
        emit(poleInfo("关闭摄像头","0"));
        ui->pushButton_camCtl->setText("打开摄像头");
    }
}

/****************** 线程类 ********************/
/* 重写 */
void MyThr::run(){
    qDebug() << __func__ << " : " << QThread::currentThreadId();

    /* 线程事件轮询 */
    exec();
}






