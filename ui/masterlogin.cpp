#include "masterlogin.h"
#include "ui_masterlogin.h"

MasterLogin::MasterLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MasterLogin)
{
    ui->setupUi(this);
}

MasterLogin::~MasterLogin()
{
    delete ui;
    qDebug("master login destrcut end");
}
