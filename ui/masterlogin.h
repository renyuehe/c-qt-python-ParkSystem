#ifndef MASTERLOGIN_H
#define MASTERLOGIN_H

#include <QWidget>

namespace Ui {
class MasterLogin;
}

class MasterLogin : public QWidget
{
    Q_OBJECT

public:
    explicit MasterLogin(QWidget *parent = nullptr);
    ~MasterLogin();

private:
    Ui::MasterLogin *ui;
};

#endif // MASTERLOGIN_H
