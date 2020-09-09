#ifndef MASTERSELECT_H
#define MASTERSELECT_H

#include <QWidget>

namespace Ui {
class MasterSelect;
}

class MasterSelect : public QWidget
{
    Q_OBJECT

public:
    explicit MasterSelect(QWidget *parent = nullptr);
    ~MasterSelect();
signals:
    void searchInfo(QString str,QString info);
private slots:
    /* 暂时写没有命令格式的 查询 测试用 */
    void on_pushButton_historySearch_clicked();

    /* 显示信息在 主窗口 */
    void showInfo(QString);

    void on_pushButton_parkStat_clicked();

private:
    Ui::MasterSelect *ui;
};

#endif // MASTERSELECT_H
