#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QMainWindow>
//network
#include <QAbstractSocket>
#include <QHostAddress>
#include <QTcpSocket>
//debug
#include <QDebug>
#include <QMessageBox>
#include <QMetaType>
#include <QString>
#include <QStandardPaths>
//
#include "mainwindow.h"

namespace Ui {
class loginForm;
}

class loginForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit loginForm(QWidget *parent = nullptr);
    ~loginForm();

signals:
     void newMessage(QString);
     void sendSocket(QTcpSocket*);

private slots:
    void on_pushButton_login_3_clicked();
    void on_pushButton_login_clicked();
    //send message to login
    void sendMessageLogin();

    void readSocket();
    //void discardSocket();
    //void displayError(QAbstractSocket::SocketError socketError);

    //void displayMessage(const QString& str);



private:
    Ui::loginForm *ui;
    QTcpSocket* socket;

    QString login{};
    QString password{};
    QString *hostAdress;
    quint16  *port{};

    QDataStream *socketStream{};
    QByteArray header{};
    QByteArray byteArray{};
    //
    MainWindow *mainWindow;


};

#endif // LOGINFORM_H
