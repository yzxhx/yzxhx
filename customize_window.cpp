#include "customize_window.h"
#include "ui_customize_window.h"
#include <QMessageBox>

Customize_window::Customize_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Customize_window)
{
    ui->setupUi(this);
    connect(ui->ok,&QPushButton::clicked,this,&Customize_window::senddata);
}

void Customize_window::senddata()
{
    QString str1=ui->lineEdit1->text();
    QString str2=ui->lineEdit2->text();
    QString str3=ui->lineEdit3->text();
    if(str1.isEmpty()||str2.isEmpty()||str3.isEmpty())
    {
        QMessageBox::warning(this,"wrong","请输入所有数值");
        return;
    }
    if(str3.toInt()<0||str3.toInt()>999)
    {
        QMessageBox::warning(this,"wrong","超出范围");
        return;
    }
    if(str1.toInt()<2||str1.toInt()>54)
    {
        QMessageBox::warning(this,"wrong","超出范围");
        return;
    }
    if(str2.toInt()<2||str2.toInt()>24)
    {
        QMessageBox::warning(this,"wrong","超出范围");
        return;
    }
    emit slot(str1.toInt(),str2.toInt(),str3.toInt());
}
Customize_window::~Customize_window()
{
    delete ui;
}
