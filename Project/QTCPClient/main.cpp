#include <QApplication>
#include "loginform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginForm w;
 
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
