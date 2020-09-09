#include "masterselect.h"
#include "ui_masterselect.h"

MasterSelect::MasterSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MasterSelect){
    ui->setupUi(this);
}

MasterSelect::~MasterSelect(){
    delete ui;
    qDebug("master select destrcut end");
}

/* 暂时写没有命令格式的 查询 测试用 */
void MasterSelect::on_pushButton_historySearch_clicked(){
    QString str;
    if(ui->radioButton_carNumber->isChecked()){
        str = ui->lineEdit_carNnmber->text();
        emit(searchInfo("查询车牌",str));
    }
    else if(ui->radioButton_name->isChecked()){
        str = ui->lineEdit_name->text();
        emit(searchInfo("查询车主",str));
    }
    else {
        ui->textBrowser->append("请选择车牌号 或者 姓名查询:");
    }
}

void MasterSelect::on_pushButton_parkStat_clicked()
{
   emit(searchInfo("查询状态",nullptr));
}

void MasterSelect::showInfo(QString str){
    //ui->textBrowser->clear();
    ui->textBrowser->setText(str);
}


