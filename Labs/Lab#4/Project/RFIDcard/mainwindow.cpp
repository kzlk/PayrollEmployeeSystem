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
  // const QHostAddress address(HOST_ADRESS);

  socket->connectToHost(Connection::IP_ADDRESS, Connection::PORT);

  if (socket->waitForConnected()) {
    ui->server_con->setText("Connected to Server");
    getEmployeeNameIdSurname();
  }

  else
    QMessageBox::critical(this, "QTCPClient",
                          QString("The following error occurred: %1.")
                              .arg(socket->errorString()));

  ui->stackedWidget->setCurrentWidget(ui->page);
  ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
  ui->dateTimeEdit_2->setDateTime(QDateTime::currentDateTime());

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
  case msg::header::getAttandanceDate:
    getAttandanceInfo(socketStream);
    break;
  case msg::setAttandanceSuccess:
    ui->comboBox->disconnect();
    ui->comboBox->clear();
    ui->comboBox->connect(ui->comboBox, &QComboBox::currentTextChanged, this,
                          &MainWindow::on_comboBox_currentTextChanged);
    QMessageBox::information(this, "QTCPClient", "Data Inserted Successfully!");
    ui->stackedWidget->setCurrentWidget(ui->page);
    getEmployeeNameIdSurname();
    break;

  case msg::setAttandanceFailure:
    QMessageBox::critical(this, "QTCPClient", "Error when inserted data!");
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
        temp.append("| " + receivedItem.toString());
      } else {
        temp.append(receivedItem.toString() + " ");
      }
    }

    receivedList.push_back(temp);
  }

  ui->comboBox->addItems(receivedList);

  // QMessageBox::information(this, "QTCPClient", "addItems(receivedList)!");
}

void MainWindow::getAttandanceInfo(QDataStream &socketStream) {
  QString rec{};
  socketStream >> rec;
  if (rec == st.empty) {
    ui->stackedWidget->setCurrentIndex(1);
  } else {

    temp = QDateTime::fromString(rec);
    ui->dateTimeEdit->setDateTime(QDateTime::fromString(rec));
    ui->label_entered->setText(rec);
    ui->stackedWidget->setCurrentIndex(0);
  }
}

void MainWindow::sendAttandanceInfo(msg::header header) {
  if (!socket)
    QMessageBox::critical(this, "QTCPClient", "Not connected");

  if (!socket->isOpen())
    QMessageBox::critical(this, "QTCPClient",
                          "Socket doesn't seem to be opened");

  QDataStream socketStream(socket);
  socketStream.setVersion(QDataStream::Qt_5_15);

  auto idEmp = ui->comboBox->currentText().split(" | ")[1];

  switch (header) {
  case msg::header::setEmployeeAttandanceEntered:
    socketStream << msg::header::setEmployeeAttandanceEntered << idEmp
                 << ui->dateTimeEdit_2->dateTime();
    break;

  case msg::header::setEmployeeAttandanceExit:
    socketStream << msg::header::setEmployeeAttandanceExit << idEmp
                 << ui->dateTimeEdit->dateTime();
    break;

  default:
    QMessageBox::critical(this, "QTCPClient",
                          "Unknown header in sendAttandanceInfo ");
    break;
  }
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

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1) {
  if (!socket)
    QMessageBox::critical(this, "QTCPClient", "Not connected");

  if (!socket->isOpen())
    QMessageBox::critical(this, "QTCPClient",
                          "Socket doesn't seem to be opened");

  auto a = ui->comboBox->currentText().split(" | ")[1];
  // QMessageBox::information(this, "QTCPServer", QString("The ID:%1.").arg(a));
  QDataStream socketStream(socket);
  socketStream.setVersion(QDataStream::Qt_5_15);
  socketStream << msg::header::getAttandanceDate << a;
}

void MainWindow::on_pushButton_clicked() {
  if (ui->dateTimeEdit->dateTime() <= temp) {
    QMessageBox::critical(this, "QTCPClient", "Incorect data selected!");
    return;
  }
  sendAttandanceInfo(msg::setEmployeeAttandanceExit);
}

void MainWindow::on_pushButton_2_clicked() {
  sendAttandanceInfo(msg::setEmployeeAttandanceEntered);
}
