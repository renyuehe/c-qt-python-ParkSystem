#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QtWidgets>
#include <QTime>

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = nullptr);
    ~Timer();

signals:
    void sendTimeToVideo(QString);

private slots:
    void timerRun(bool *loop);
public:
    bool flagRun = 1;
private:

};

#endif // TIMER_H
