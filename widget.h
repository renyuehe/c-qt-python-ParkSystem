#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <QtWidgets>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ui/video.h"
#include "ui/advertchargevideo.h"
#include "ui/masterlogin.h"
#include "ui/mastervideo.h"
#include "ui/masterselect.h"

#include "functiontool/timer.h"
#include "functiontool/advertincharge.h"
#include "functiontool/flow.h"
#include "functiontool/control.h"
#include "qrencode/qrencode.h"

#include "config.h"/*全局配置*/

/* ***************************** 线层类 **************************** */
class MyThr : public QThread{
    Q_OBJECT
public:
    explicit MyThr(QObject *parent = nullptr):
        QThread (parent)
    {
        qDebug() << __func__ << " : " << QThread::currentThreadId();
    }
    ~MyThr(){
        qDebug()<<__func__<<":"<<QThread::currentThread()<<": destruct";
    }

private slots:


private:
    /* 线层执行函数 */
    void run();
};

/* *************************** 主窗口 ******************************* */
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    /* 重写事件 */
    void closeEvent(QCloseEvent *e);
    /* 重写绘图事件 */
    void paintEvent(QPaintEvent *event);

private slots:
    void quit();
    /* 运行 停止 按钮 */
    void on_pushButton_start_clicked();
    /* 显示video按钮 */
    void on_pushButton_video_clicked();
    /* 显示login按钮 */
    void on_pushButton_login_clicked();
    /* 显示select按钮 */
    void on_pushButton_select_clicked();

    /* 激活(双击 或 其他)任务栏图标槽函数 */
    void slotActivated(QSystemTrayIcon::ActivationReason);

    /* 连接 或 断开 */
    void on_pushButton_connect_clicked();

    void connectOvertime();

    void setBtnDisconnectEnable();
    void setBtnConnectEnable();

    /* 杆子 控制 */
    void on_pushButton_pole_clicked();
    /* 摄像头 控制 */
    void on_pushButton_camCtl_clicked();

    /* advertincharge 处理逻辑 */
    void dealQrencode(QString);

    void on_pushButton_testEcho_clicked();

signals:
    /* 开始运行时间 */
    void timerStart(bool *loop);
    /* 开始运行广告 */
    void advertStart();
    /* 开始视频流时间 信号 */
    void flowStart(bool *loop);
    void connectTcp(bool isConnect);

    /* 杆控信号 */
    void poleInfo(QString,QString);

    /* connect 至 qApp::quit() */
    void kill();

private:
    Ui::Widget *ui;
    /* 任务栏图标 */
    QSystemTrayIcon *mySystemTrayIcon;/* 任务栏图标 */
    QMenu *myMenu;/* 右键弹出菜单 */
    QAction *myAction;

    /* 线程 */
    MyThr *myThrFlow;   /* 视频流线程 */
    MyThr *myThrTimer;  /* 时间流线程 */
    MyThr *myThrControl;/* 控制流线程 */
    MyThr *myThrChargeAdvert;

    /* 耗时类 */
    Timer *myTimer;         /* 时间类   */
    Flow *myFlow;           /* 视频流类 */
    Control *myControl;     /* Tcp控制类 */
    AdvertInCharge *myChargeAdvert; /* 广告滚动 */

    /* 判断 */
    bool loopTimer;     /* 判断时间循环 */
    bool loopFlow;      /* 判断视频循环 */
    bool isConnect;     /* tcp 条件 */

    /* 显示框架类 */
    Video *myVideo;     /* 外部视频流独立窗口 */
    AdvertChargeVideo *myChargeAdvertVideo;   /* 内部收费独立窗口:二维码 */
    MasterLogin *myMasterLogin; /* 控制台注册窗口 */
    MasterVideo *myMasterVideo; /* 控制台视频流窗口 */
    MasterSelect *myMasterSelect;/* 控制太查询窗口 */
};

#endif // WIDGET_H
