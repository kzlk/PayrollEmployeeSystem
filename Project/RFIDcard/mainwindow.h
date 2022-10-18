#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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

private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  QDataStream *socketStream{};
  void readSocket();
  void discardSocket();
  void displayError(QAbstractSocket::SocketError socketError);

private:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  QPoint mousePoint;
  bool isMouseDown = false;
  void on_minimizeButton_clicked();
  void on_closeButton_clicked();
};
#endif // MAINWINDOW_H
