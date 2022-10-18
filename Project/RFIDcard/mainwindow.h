#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#define HOST_ADRESS "127.0.0.1"
#define HOST_PORT 8080

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

private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  QDataStream *socketStream{};
  QVector<QString> listOfEmpID{};
  void readSocket();
  void discardSocket();
  void displayError(QAbstractSocket::SocketError socketError);

  void getEmployeeNameIdSurname();
  void appendDataToComboBox(QDataStream &socketStream);

private:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  QPoint mousePoint;
  bool isMouseDown = false;
};
#endif // MAINWINDOW_H
