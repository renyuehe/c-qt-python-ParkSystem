#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QFont>
#include <QPalette>
#include "functiontool/flow.h"

namespace Ui {
class Video;
}

/* 面对车辆的视频框架类 */
class Video : public QWidget
{
    Q_OBJECT
public:
    explicit Video(QWidget *parent = nullptr);
    ~Video();

private:
    void closeEvent(QCloseEvent *e);

private slots:
    /* 显示当前时间 */
    void timeDisplay(QString);
    void picDisplay(QPixmap pixmap);
private:
    Ui::Video *ui;

    QFont *font;
    QPalette *pa;
};

#endif // VIDEO_H
