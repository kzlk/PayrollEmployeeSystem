#include "mainwindow.h"
#include "admin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    admin admin;
    admin.show();
    return a.exec();
}
