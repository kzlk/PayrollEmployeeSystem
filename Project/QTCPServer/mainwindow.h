#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QSet>
#include <QStandardPaths>
#include <QTcpServer>
#include <QTcpSocket>
#include "dbUtils.cpp"
#include "CMessage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT //can create slot and signal

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newMessage(QString);

private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket* socket);
    void readSocket(); //socket readiness to receive data
    void discardSocket(); //disconnect client from socket
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(const QString& str);

private:
    void sendTotalInfoEmployee(QTcpSocket* socket, int&, QString&, QString&);
    void sendInitialData(QTcpSocket* socket, QSqlQueryModel& model);
    void sendAutorizationStatus(QTcpSocket* socket, bool isAutorized);
    void sendEmpDept(QTcpSocket* socket);
    void sendEmpDesig(QTcpSocket* socket);


    Ui::MainWindow *ui;
    DatabaseUtils dbUtils;
    QTcpServer* m_server;
    QSet<QTcpSocket*> connection_set;
    msg::header hd;
    msg::status st;
};

#endif // MAINWINDOW_H
