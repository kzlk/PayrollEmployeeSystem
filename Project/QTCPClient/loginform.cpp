#include "loginform.h"
#include "ui_loginform.h"

loginForm::loginForm(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::loginForm) {
  ui->setupUi(this);

  socket = new QTcpSocket(this);

  // connect(this, &loginForm::newMessage, this, &loginForm::displayMessage);
  connect(socket, &QTcpSocket::readyRead, this, &loginForm::readSocket);
  // connect(socket, &QTcpSocket::disconnected, this,
  // &loginForm::discardSocket);
  // connect(socket, &QAbstractSocket::errorOccurred, this,
  // &loginForm::displayError);
  // mainWindow = new MainWindow();
}

// close socket and delete ui
loginForm::~loginForm() {
  if (socket->isOpen())
    socket->close();
  delete ui;
}

// read data from server
void loginForm::readSocket() {
  QByteArray buffer;
  buffer.clear();
  QDataStream socketStream(socket);

  socketStream.setVersion(QDataStream::Qt_5_15);

  socketStream.startTransaction();
  // read header
  socketStream >> buffer;

  if (hd.autorazation == buffer) {
    socketStream >> buffer;
    if (buffer.toStdString() == st.success.toStdString()) {
      // mainWindown.
      disconnect(socket, &QTcpSocket::readyRead, this, &loginForm::readSocket);
      mainWindow = new MainWindow();
      connect(this, &loginForm::sendSocket, mainWindow,
              &MainWindow::receiveSocket);
      emit sendSocket(socket); // send socket to main window
      mainWindow->show();
      this->close();
    } else {
      QMessageBox::critical(this, "QTCPClient",
                            "Password or Login is incorrect. Please try again");
    }
  } else {
    QMessageBox::information(this, "QTCPCLIENT", "header != autorazation!");
  }

  if (!socketStream.commitTransaction()) {
    QString message = QString("%1 :: Waiting for more data to come..")
                          .arg(socket->socketDescriptor());
    QMessageBox::information(this, "QTCPCLIENT", message);
    // emit newMessage(message);
    return;
  }
}

// button connect to server
void loginForm::on_pushButton_login_3_clicked() {

  hostAdress = new QString(ui->ilinEdit_pAdress->text());
  port = new quint16(ui->lineEdit_port->text().toInt());
  const QHostAddress address(*hostAdress);

  socket->connectToHost(address, *port);

  if (socket->waitForConnected())
    ui->statusbar->showMessage("Connected to Server");
  else {
    QMessageBox::critical(this, "QTCPClient",
                          QString("The following error occurred: %1.")
                              .arg(socket->errorString()));
  }
}

void loginForm::sendMessageLogin() {
  if (socket) {
    if (socket->isOpen()) {
      login = ui->lineEdit_login->text();
      password = ui->lineEdit_password->text();
      socketStream = new QDataStream(socket);
      socketStream->setVersion(QDataStream::Qt_5_15);

      // send to server
      *socketStream << hd.autorazation.toUtf8() << login.toUtf8()
                    << password.toUtf8();
    } else {
      QMessageBox::critical(this, "QTCPClient",
                            "Socket doesn't seem to be opened");
    }
  } else {
    QMessageBox::critical(this, "QTCPClient", "Not connected");
  }
}

// button autorazation
void loginForm::on_pushButton_login_clicked() { sendMessageLogin(); }

//

void loginForm::on_minimizeButton_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}


void loginForm::on_closeButton_clicked()
{
     this->close();
}

