#include "dialog_code.h"
#include "ui_dialog_code.h"
#include "QMessageBox"
Dialog_code::Dialog_code(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_code)
{
    ui->setupUi(this);
}

Dialog_code::~Dialog_code()
{
    delete ui;
}

void Dialog_code::on_pushButton_clicked()
{

    if(ui->lineEdit_2->text()=="1234"){
        QMessageBox::information(this,"  ","设备连接成功");
        flag=1;
        this->close();
    }else
        QMessageBox::information(this,"  ","密码错误，请重新输入");

}

void Dialog_code::on_pushButton_2_clicked()
{
    this->close();
}
