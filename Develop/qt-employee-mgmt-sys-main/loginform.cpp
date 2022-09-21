#include "loginform.h"
#include "ui_loginform.h"

loginForm::loginForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginForm)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    //connect(this, &loginForm::newMessage, this, &loginForm::displayMessage);
   connect(socket, &QTcpSocket::readyRead, this, &loginForm::readSocket);
   // connect(socket, &QTcpSocket::disconnected, this, &loginForm::discardSocket);
    //connect(socket, &QAbstractSocket::errorOccurred, this, &loginForm::displayError);
   mainWindow = new MainWindow();
   connect(this, &loginForm::sendSocket, mainWindow ,&MainWindow::receiveSocket);


}

loginForm::~loginForm()
{
    if(socket->isOpen())
        socket->close();
    delete ui;
}

void loginForm::readSocket()
{
     QByteArray buffer;
     QDataStream socketStream(socket);

     socketStream.setVersion(QDataStream::Qt_5_15);

     socketStream.startTransaction();
     socketStream >> buffer;


     if(!socketStream.commitTransaction())
     {
         QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());
         emit newMessage(message);
         return;
     }

     QString header = "autorization";

     if(header == "autorization")
     {
        if(buffer.toStdString() == "success")
        {
            emit sendSocket(socket);
            mainWindow->show();
            this->close();
        }
     }

}

//button connect to server
void loginForm::on_pushButton_login_3_clicked()
{

    hostAdress = new QString(ui->ilinEdit_pAdress->text());
    port = new quint16(ui->lineEdit_port->text().toInt());
    const QHostAddress address (*hostAdress);

    socket->connectToHost(address, *port);

    if(socket->waitForConnected())
        ui->statusbar->showMessage("Connected to Server");
    else
    {
        QMessageBox::critical(this,"QTCPClient", QString("The following error occurred: %1.").arg(socket->errorString()));
    }
}

void loginForm::sendMessageLogin()
{
    if(socket)
    {
        if(socket->isOpen())
        {
            login = ui->lineEdit_login->text();
            password = ui->lineEdit_password->text();
            socketStream = new QDataStream(socket);
            socketStream->setVersion(QDataStream::Qt_5_15);
            QString h = "autorization";
           // header.prepend("autorization");
           //header.resize(20);
            *socketStream << h.toUtf8() << login.toUtf8() << password.toUtf8();

        }else
        {
            QMessageBox::critical(this,"QTCPClient","Socket doesn't seem to be opened");
        }
    }else
    {
         QMessageBox::critical(this,"QTCPClient","Not connected");
    }
}

//button autorazation
void loginForm::on_pushButton_login_clicked()
{
    sendMessageLogin();
}

