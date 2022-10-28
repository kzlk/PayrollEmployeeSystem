#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CMessage.h"
#include "qdatetime.h"

#include <QMainWindow>
#include <QTcpSocket>

#include "../Constant/Connection.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_closeButton_clicked();

  void on_minimizeButton_clicked();

  void on_comboBox_currentTextChanged(const QString &arg1);

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

private:
  QDateTime temp{};
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  QDataStream *socketStream{};
  void readSocket();
  void discardSocket();
  void displayError(QAbstractSocket::SocketError socketError);

  void getEmployeeNameIdSurname();
  void appendDataToComboBox(QDataStream &socketStream);
  void getAttandanceInfo(QDataStream &socketStream);
  void sendAttandanceInfo(msg::header header);

private:
  // msg::header hd;
  msg::status st;
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  QPoint mousePoint;
  bool isMouseDown = false;
};
#endif // MAINWINDOW_H
