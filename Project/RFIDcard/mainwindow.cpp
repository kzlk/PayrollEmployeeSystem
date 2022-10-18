#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  socket = new QTcpSocket(this);

  connect(socket, &QAbstractSocket::errorOccurred, this,
          &MainWindow::displayError);
  connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
  connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
}

MainWindow::~MainWindow() {

  if (socket->isOpen())
    socket->close();
  delete ui;
}

void MainWindow::readSocket() {}

void MainWindow::discardSocket() {}

void MainWindow::displayError(QAbstractSocket::SocketError socketError) {
  switch (socketError) {
  case QAbstractSocket::RemoteHostClosedError:
    break;
  case QAbstractSocket::HostNotFoundError:
    QMessageBox::information(this, "QTCPServer",
                             "The host was not found. Please check the host "
                             "name and port settings.");
    break;
  case QAbstractSocket::ConnectionRefusedError:
    QMessageBox::information(
        this, "QTCPServer",
        "The connection was refused by the peer. Make sure QTCPServer is "
        "running, and check that the host name and port settings are correct.");
    break;
  default:
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QMessageBox::information(this, "QTCPServer",
                             QString("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    break;
  }
}

// minimaze window
void MainWindow::on_minimizeButton_clicked() {
  this->setWindowState(Qt::WindowMinimized);
}

// close window
void MainWindow::on_closeButton_clicked() { this->close(); }

/*FOR MOUSE EVENT*/
void MainWindow::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    isMouseDown = true;
    mousePoint = event->globalPos();
  }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) { isMouseDown = false; }

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
  const QPoint delta = event->globalPos() - mousePoint;
  if (isMouseDown == true)
    move(x() + delta.x(), y() + delta.y());
  else
    move(x() + delta.x(), y() + delta.y());
  mousePoint = event->globalPos();
}
