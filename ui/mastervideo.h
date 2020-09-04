#ifndef MASTERVIDEO_H
#define MASTERVIDEO_H

#include <QWidget>

namespace Ui {
class MasterVideo;
}

class MasterVideo : public QWidget
{
    Q_OBJECT

public:
    explicit MasterVideo(QWidget *parent = nullptr);
    ~MasterVideo();

private slots:
    void timeDisplay(QString str);

    void picDisplay(QPixmap pixmap);

private:
    Ui::MasterVideo *ui;

    QFont *font;
    QPalette *pa;
};

#endif // MASTERVIDEO_H
