#ifndef CHARGE_H
#define CHARGE_H

#include <QWidget>
#include <QPixmap>
#include <QDebug>
#include <QCloseEvent>
#include "qrencode/qrencode.h"

namespace Ui {
class charge;
}

class AdvertChargeVideo : public QWidget
{
    Q_OBJECT

public:
    explicit AdvertChargeVideo(QWidget *parent = nullptr);
    ~AdvertChargeVideo();

public slots:
    void picShow(int i=-1);
    void codeShow(QPixmap);

private:
    void closeEvent(QCloseEvent *e);

private:
    Ui::charge *ui;
    /* 每一张图片:空间换时间 */
    QPixmap chargePix;
    QString picStr;

    /* 二维码 */
};

#endif // CHARGE_H
