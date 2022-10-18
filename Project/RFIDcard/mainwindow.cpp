#include "mainwindow.h"
#include "CMessage.h"
#include "ui_mainwindow.h"
#include <QAbstractSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  const QHostAddress address(HOST_ADRESS);

  socket->connectToHost(address, HOST_PORT);

  if (socket->waitForConnected()) {
    ui->statusBar->showMessage("Connected to Server");
    getEmployeeNameIdSurname();
  }

  else
    QMessageBox::critical(this, "QTCPClient",
                          QString("The following error occurred: %1.")
                              .arg(socket->errorString()));

  ui->stackedWidget->setCurrentWidget(ui->page);

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

void MainWindow::readSocket() {
  QDataStream socketStream(socket);
  socketStream.setVersion(QDataStream::Qt_5_15);
  quint8 headCommand{};

  socketStream.startTransaction();

  socketStream >> headCommand;

  switch (headCommand) {
  case msg::header::getEmployeeNameIdSurname:
    appendDataToComboBox(socketStream);
    break;
  default:
    QMessageBox::information(this, "QTCPCLIENT", "received unknows header");
    break;
  }

  if (!socketStream.commitTransaction()) {
    qDebug() << "Waiting for more data to come";
    return;
  }
}

void MainWindow::discardSocket() {
  socket->deleteLater();
  socket = nullptr;
}

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
    QMessageBox::information(this, "QTCPServer",
                             QString("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    break;
  }
}

void MainWindow::getEmployeeNameIdSurname() {
  if (!socket)
    QMessageBox::critical(this, "QTCPClient", "Not connected");

  if (!socket->isOpen())
    QMessageBox::critical(this, "QTCPClient",
                          "Socket doesn't seem to be opened");

  QDataStream socketStream(socket);
  socketStream.setVersion(QDataStream::Qt_5_15);
  socketStream << msg::header::getEmployeeNameIdSurname;
}

void MainWindow::appendDataToComboBox(QDataStream &socketStream) {
  quint32 row{};
  quint32 column{};
  socketStream >> row >> column;
  qDebug() << "\nRow received = " << row << " | Column received = " << column;

  ui->comboBox->clear();
  QStringList receivedList{};
  int max{};
  for (int r = 0; r < row; ++r) {
    QString temp{};
    temp.clear();
    for (int col = 0; col < column; ++col) {
      QVariant receivedItem{};
      socketStream >> receivedItem;
      qDebug() << receivedItem;
      if (col == column - 1) {
        listOfEmpID.push_back(receivedItem.toString());
        temp.append("| " + receivedItem.toString());
      } else {
        temp.append(receivedItem.toString() + " ");
      }
    }

    receivedList.push_back(temp);
    if (max < temp.length())
      max = temp.length() + 5;
  }

  ui->comboBox->addItems(receivedList);

  // auto res = ui->comboBox->sizeHint() + 15 * ui->comboBox->frameSize();
  // ui->comboBox->setFixedSize(res);
}

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

void MainWindow::on_closeButton_clicked() { this->close(); }

void MainWindow::on_minimizeButton_clicked() {
  this->setWindowState(Qt::WindowMinimized);
}
