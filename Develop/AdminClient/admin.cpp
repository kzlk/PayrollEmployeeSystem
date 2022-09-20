#include "admin.h"
#include "ui_admin.h"
#include <QMessageBox>
admin::admin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::admin)
{
    ui->setupUi(this);
}

admin::~admin()
{
    delete ui;
}

void admin::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_login->text();
    QString password =  ui->lineEdit_password->text();

    if(username == "test" && password == "test")
    {
        QMessageBox::information(this, "Login", "Login and password is correct");
        hide();
        mainForm = new MainWindow(this);
        mainForm->show();

    }
}

