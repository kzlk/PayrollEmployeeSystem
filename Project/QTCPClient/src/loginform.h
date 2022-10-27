#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QMainWindow>
// network
#include <QAbstractSocket>
#include <QHostAddress>
#include <QTcpSocket>
// debug
#include <QDebug>
#include <QMessageBox>
#include <QMetaType>
#include <QStandardPaths>
#include <QString>
//
#include "CMessage.h"
#include "mainwindow.h"

namespace Ui
{
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
    void sendSocket(QTcpSocket *, quint64 &uniqueID);

  private slots:
    void on_pushButton_login_clicked();
    // send message to login
    void sendMessageLogin();

    void readSocket();
    void discardSocket();
    // void displayError(QAbstractSocket::SocketError socketError);

    void on_minimizeButton_clicked();

    void on_closeButton_clicked();

    void on_closeButton_2_clicked();

  private:
    Ui::loginForm *ui;
    QTcpSocket *socket;

    QString login{};
    QString password{};
    QString *hostAdress;
    quint16 *port{};

    QDataStream *socketStream{};
    QByteArray header{};
    QByteArray byteArray{};
    //
    MainWindow *mainWindow = nullptr;
    msg::header hd{};
    quint64 myUniqueId{};

    msg::status st{};

  private:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint mousePoint;
    bool isMouseDown = false;
};

#endif // LOGINFORM_H
