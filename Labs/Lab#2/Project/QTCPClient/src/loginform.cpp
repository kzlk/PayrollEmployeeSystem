#include "loginform.h"
#include "../Constant/Connection.h"
#include "ui_loginform.h"

loginForm::loginForm(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::loginForm)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &loginForm::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &loginForm::discardSocket);
    // connect(socket, &QAbstractSocket::errorOccurred, this,
    // &loginForm::displayError);

    socket->connectToHost(Connection::IP_ADDRESS, Connection::PORT);

    if (socket->waitForConnected())
    {
        ui->stackedWidget->setCurrentWidget(ui->page);
        ui->statusbar->showMessage("Connected to Server");
    }

    else
    {
        QMessageBox::critical(this, "QTCPClient",
                              QString("The following error occurred: %1.")
                                  .arg(socket->errorString()));
        QApplication::quit();
        ui->stackedWidget->setCurrentWidget(ui->page_2);
    }
}

// close socket and delete ui
loginForm::~loginForm()
{
    if (socket->isOpen())
        socket->close();
    delete ui;
}

// read data from server
void loginForm::readSocket()
{
    QByteArray buffer;
    buffer.clear();
    QDataStream socketStream(socket);

    quint8 headCommand{};
    ;
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    // read header
    // socketStream >> buffer;

    socketStream >> headCommand;

    if (headCommand == msg::header::autorazation)
    {

        socketStream >> buffer;
        if (buffer.toStdString() == st.success.toStdString())
        {

            socketStream >> myUniqueId;
            // mainWindown.
            disconnect(socket, &QTcpSocket::readyRead, this,
                       &loginForm::readSocket);
            mainWindow = new MainWindow();
            mainWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
            connect(this, &loginForm::sendSocket, mainWindow,
                    &MainWindow::receiveSocket);
            emit sendSocket(socket, myUniqueId); // send socket to main window
            mainWindow->show();
            this->close();
        }
        else if (buffer.toStdString() == st.failure.toStdString())
        {
            QMessageBox::critical(
                this, "QTCPClient",
                "Password or Login is incorrect. Please try again");
        }
        else if (buffer.toStdString() == st.unknownError.toStdString())
        {
            QMessageBox::critical(
                this, "QTCPClient",
                "Server can not autorizate you now! Please try again!");
        }
        else if (buffer.toStdString() == st.alreadyAutorized.toStdString())
        {
            QMessageBox::critical(
                this, "QTCPClient",
                "You already autorization! Please log out from all "
                "account and try again!");
        }
    }
    else
    {
        QMessageBox::information(this, "QTCPCLIENT", "header != autorazation!");
    }

    if (!socketStream.commitTransaction())
    {
        QString message = QString("%1 :: Waiting for more data to come..")
                              .arg(socket->socketDescriptor());
        QMessageBox::information(this, "QTCPCLIENT", message);
        // emit newMessage(message);
        return;
    }
}

void loginForm::discardSocket()
{
    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);
    socketStream << msg::header::disconnect << myUniqueId;
    socket->deleteLater();
    socket = nullptr;
}

void loginForm::sendMessageLogin()
{
    if (socket)
    {
        if (socket->isOpen())
        {

            login = ui->lineEdit_login->text();
            password = ui->lineEdit_password->text();

            if (login.trimmed().isEmpty() || password.trimmed().isEmpty())
            {
                QMessageBox::information(this, "QTCPClient",
                                         "The all fields must be filled");
                return;
            }

            socketStream = new QDataStream(socket);
            socketStream->setVersion(QDataStream::Qt_5_15);

            // send to server
            *socketStream << msg::header::autorazation << login.toUtf8()
                          << password.toUtf8();
        }
        else
        {
            QMessageBox::critical(this, "QTCPClient",
                                  "Socket doesn't seem to be opened");
        }
    }
    else
    {
        QMessageBox::critical(this, "QTCPClient", "Not connected");
    }
}

// button autorazation
void loginForm::on_pushButton_login_clicked()
{
    sendMessageLogin();
}

// minimaze window
void loginForm::on_minimizeButton_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

// close window
void loginForm::on_closeButton_clicked()
{
    this->close();
}

/*FOR MOUSE EVENT*/
void loginForm::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isMouseDown = true;
        mousePoint = event->globalPos();
    }
}

void loginForm::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseDown = false;
}

void loginForm::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint delta = event->globalPos() - mousePoint;
    if (isMouseDown == true)
        move(x() + delta.x(), y() + delta.y());
    else
        move(x() + delta.x(), y() + delta.y());
    mousePoint = event->globalPos();
}
