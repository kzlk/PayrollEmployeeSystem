#ifndef ADMIN_H
#define ADMIN_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class admin;
}

class admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::admin *ui;
    MainWindow *mainForm;
};

#endif // ADMIN_H
