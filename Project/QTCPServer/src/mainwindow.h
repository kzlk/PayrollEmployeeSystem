#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CMessage.h"
//#include "dbUtils.cpp"
#include "cautopilot.h"
#include "cgeneratepdf.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMetaType>
#include <QSet>
#include <QStandardPaths>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT // can create slot and signal

        public : explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  signals:
    void newMessage(QString);

  private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket *socket);
    void readSocket();    // socket readiness to receive data
    void discardSocket(); // disconnect client from socket
    void displayError(QAbstractSocket::SocketError socketError);
    void displayMessage(const QString &str);

  private:
    void sendTotalInfoEmployee(QTcpSocket *socket, int &, QString &, QString &);
    void sendInitialData(msg::header header, QTcpSocket *socket,
                         QSqlQueryModel &model);
    void sendAutorizationStatus(QTcpSocket *socket, bool isAutorized,
                                QString &login);
    void sendEmpDept(QTcpSocket *socket);
    void sendEmpDesig(QTcpSocket *socket, QString &deptName);
    void sendUnAutorizedStatus(QTcpSocket *socket);
    void sendSalary(QTcpSocket *socket, QString &empDemt, QString &empDesig);
    void sendEmployeeID(QTcpSocket *socket, QString &empDept,
                        QString &empDesig);
    void addEmployeeToDB(QTcpSocket *socket, QVariantList &employeeInfo);

    // RFID Emulator
    void sendNameIdSurname(QTcpSocket *socket, QSqlQueryModel &model);
    void sendAttamdanceInfo(QTcpSocket *socket, QString &Id);
    void sendStatusInsertEntered(QTcpSocket *socket, QDateTime &enterDate,
                                 QString &empId);
    void sendStatusUpdateExit(QTcpSocket *socket, QDateTime &exitDate,
                              QString &empId);

    void sendStatusInsertedInSetting(QTcpSocket *socket, QDateTime startPeriod,
                                     QDateTime endPeriod, QDateTime nextPayment,
                                     qint8 autopilot, qint8 frequency);

    void sendSettingInfo(QTcpSocket *socket);

    void sendPaymentPeriodData(QTcpSocket *socket);

    void sendPaymentInfoDetail(QTcpSocket *socket, int id);

    void sendDataForPdfReport(QTcpSocket *socket, QString empId);

    void sendSearchedEmployee(QTcpSocket *socket, QString searchedText);

    void sendDataEmpDetail(QTcpSocket *socket, QString empId);

    void deleteEmployee(QTcpSocket *socket, QString empId);

    quint64 getUniqueNum();

    Ui::MainWindow *ui;
    DatabaseUtils dbUtils;
    QTcpServer *m_server;
    QMap<QTcpSocket *, QString> setOfConnectionUser;
    msg::status st;
    CAutoPilot *myAutoPilot{};
};

#endif // MAINWINDOW_H
