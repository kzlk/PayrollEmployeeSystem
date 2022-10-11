#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlRecord>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_server = new QTcpServer();

    if(m_server->listen(QHostAddress::Any, 8080)) //start server
    {
       connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
       connect(m_server, &QTcpServer::newConnection, this, &MainWindow::newConnection);
       ui->statusBar->showMessage("Server is listening...");
    }
    else
    {
        QMessageBox::critical(this,"QTCPServer",QString("Unable to start the server: %1.").arg(m_server->errorString()));
        exit(EXIT_FAILURE);
    }

    if(dbUtils.connectToDB())
    {

        displayMessage("Connected to database "   + dbUtils.dbName +  " sucessufully");
    }
    else
    {
        displayMessage("Connected to database "   + dbUtils.dbName +  " failure");
    }
}

MainWindow::~MainWindow()
{
    foreach (QTcpSocket* socket, connection_set)
    {
        socket->close();
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

    delete ui;
}

void MainWindow::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

void MainWindow::appendToSocketList(QTcpSocket* socket)
{
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);
    ui->comboBox_receiver->addItem(QString::number(socket->socketDescriptor()));
    displayMessage(QString("INFO :: Client with sockd:%1 has just entered the room").arg(socket->socketDescriptor()));
    //add Date to connected client
}

void MainWindow::readSocket()
{
    if(dbUtils.db.isOpen())
    {
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer;
    buffer.clear();

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> buffer;

    QString header(buffer);
    QString messageHd = QString("%1 ::<Received HEADER> -> "  + header).arg(socket->socketDescriptor());
    emit newMessage(messageHd);
     //autorization
    if(header == hd.autorazation)
    {
        socketStream >> buffer;
        QString login(buffer);
        socketStream >> buffer;
        QString password(buffer);
            sendAutorizationStatus(socket, dbUtils.checkUserIsExist(login, password));

//        if(!socketStream.commitTransaction())
//        {
//            QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());
//            emit newMessage(message);
//            return;
//        }

    }
    //employee info
    else if(header == hd.employeeDetail)
    {
        int lastID = dbUtils.getTotalEmployee().toInt();

        auto employeeDetail = dbUtils.getEmployeeDetails();

        QString dept = dbUtils.getTotalDept();

        QString designTtl = dbUtils.getTotalDesign();


        //send total info of emloyee
        sendTotalInfoEmployee(socket, lastID, dept, designTtl);

        //send employee info
        sendInitialData(socket, *employeeDetail);
    }
    else if(header == hd.infoDemp)
    {

        sendEmpDept(socket);
    }
    else
    {
        QString message = "Received header does not much any of existing header: " + header ;
        emit newMessage(message);
        return;
    }

    if(!socketStream.commitTransaction())
    {
        QString message = QString("%1 :: Waiting for more data to come.. after all condition").arg(socket->socketDescriptor());
        emit newMessage(message);
       // do
        //{
            //buffer.clear();
           // socketStream >> buffer;
            QString smth(buffer);
            emit newMessage("Comming data: --> " + smth);
       // }while(!socketStream.commitTransaction());

        return;
    }

}
    else
    {
      QString message = QString("Data base is could not open");
      emit newMessage(message);
    }
}

void MainWindow::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){
        displayMessage(QString("INFO :: A client has just left the room").arg(socket->socketDescriptor()));
        connection_set.remove(*it);
    }
    refreshComboBox();
    
    socket->deleteLater();
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, "QTCPServer", "The host was not found. Please check the host name and port settings.");
        break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, "QTCPServer", "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.");
        break;
        default:
            QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
            QMessageBox::information(this, "QTCPServer", QString("The following error occurred: %1.").arg(socket->errorString()));
        break;
    }
}

void MainWindow::on_pushButton_sendMessage_clicked()
{
    QString receiver = ui->comboBox_receiver->currentText();

    if(receiver=="Broadcast")
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            sendMessage(socket);
        }
    }
    else
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            if(socket->socketDescriptor() == receiver.toLongLong())
            {
                sendMessage(socket);
                break;
            }
        }
    }
    ui->lineEdit_message->clear();
}

void MainWindow::on_pushButton_sendAttachment_clicked()
{
    QString receiver = ui->comboBox_receiver->currentText();

    QString filePath = QFileDialog::getOpenFileName(this, ("Select an attachment"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), ("File (*.json *.txt *.png *.jpg *.jpeg)"));

    if(filePath.isEmpty()){
        QMessageBox::critical(this,"QTCPClient","You haven't selected any attachment!");
        return;
    }

    if(receiver=="Broadcast")
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            sendAttachment(socket, filePath);
        }
    }
    else
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            if(socket->socketDescriptor() == receiver.toLongLong())
            {
                sendAttachment(socket, filePath);
                break;
            }
        }
    }
    ui->lineEdit_message->clear();
}

void MainWindow::sendMessage(QTcpSocket* socket)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString str = ui->lineEdit_message->text();

            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(str.size()).toUtf8());
            header.resize(128);

            QByteArray byteArray = str.toUtf8();
            byteArray.prepend(header);

            socketStream.setVersion(QDataStream::Qt_5_15);
            socketStream << byteArray;
        }
        else
            QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPServer","Not connected");
}

void MainWindow::sendAttachment(QTcpSocket* socket, QString filePath)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QFile m_file(filePath);
            if(m_file.open(QIODevice::ReadOnly)){

                QFileInfo fileInfo(m_file.fileName());
                QString fileName(fileInfo.fileName());

                QDataStream socketStream(socket);
                socketStream.setVersion(QDataStream::Qt_5_15);

                QByteArray header;
                header.prepend(QString("fileType:attachment,fileName:%1,fileSize:%2;").arg(fileName).arg(m_file.size()).toUtf8());
                header.resize(128);

                QByteArray byteArray = m_file.readAll();
                byteArray.prepend(header);

                socketStream << byteArray;
            }else
                QMessageBox::critical(this,"QTCPClient","Couldn't open the attachment!");
        }
        else
            QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPServer","Not connected");
}

void MainWindow::sendAutorizationStatus(QTcpSocket* socket, bool isAutorized)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QDataStream socketStream(socket);
             socketStream.setVersion(QDataStream::Qt_5_15);
            if(isAutorized)
            {
                socketStream << hd.autorazation.toUtf8() << st.success.toUtf8();
            }
            else
            {
                socketStream << hd.autorazation.toUtf8() << st.failure.toUtf8();
            }
        }
        else
            QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPServer","Not connected");

}

void MainWindow::sendEmpDept(QTcpSocket *socket)
{
    if(socket)
    {
        if(socket->isOpen())
        {
             QDataStream socketStream(socket);
             socketStream.setVersion(QDataStream::Qt_5_15);

            auto empDept = dbUtils.getDepartmentList();
            for(int i = 0; i < empDept->rowCount(); ++i)
            {
                emit newMessage("Dept item ->  "  + empDept->record(i).value(0).toString().toUtf8());
                qDebug() << "| " <<  quint16(empDept->columnCount()) << "| "  << quint16(empDept->rowCount());
            }
            emit newMessage(&": row in dept -> " [ (empDept->columnCount())]);
            if(empDept->rowCount() > 0)
            {
                //qint32 a = 20;
               // socketStream  << a;
                socketStream <<   hd.infoDemp.toUtf8() << qint32(empDept->rowCount());
               // socketStream  <<  hd.totalDepartment.toUtf8();
                //socketStream << "DATYZADOVBAV";

                for(int i = 0; i < empDept->rowCount(); ++i)
                {
                     socketStream << empDept->record(i).value(0).toString().toUtf8();
                }
            }
            else
            {
                socketStream << hd.infoDemp.toUtf8() << (empDept->rowCount());
            }

        }
    else
        QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
}
else
    QMessageBox::critical(this,"QTCPServer","Not connected");
}

void MainWindow::displayMessage(const QString& str)
{
    ui->textBrowser_receivedMessages->append(str);
}

void MainWindow::refreshComboBox(){
    ui->comboBox_receiver->clear();
    ui->comboBox_receiver->addItem("Broadcast");
    foreach(QTcpSocket* socket, connection_set)
        ui->comboBox_receiver->addItem(QString::number(socket->socketDescriptor()));
}

void MainWindow::sendTotalInfoEmployee(QTcpSocket *socket, int &  lastID, QString & dept , QString & designTtl)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QDataStream socketStream(socket);
             socketStream.setVersion(QDataStream::Qt_6_3);

             //send header , last ID, dept, desighTotal
             socketStream << hd.totalInfoEmployee.toUtf8() << quint32(lastID) << dept.toUtf8() << designTtl.toUtf8();
             //
             emit newMessage("Sending header -> " + hd.totalInfoEmployee.toUtf8());
        }
        else
            QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPServer","Not connected");
}

void MainWindow::sendInitialData(QTcpSocket *socket, QSqlQueryModel &model)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QDataStream socketStream(socket);
             socketStream.setVersion(QDataStream::Qt_5_15);

             //count row and column
             socketStream << hd.employeeDetail.toUtf8() <<  quint32(model.rowCount()) << quint32(model.columnCount());

             if( model.rowCount() > 0 && model.columnCount() > 0)
             {
             //send data employee
             for(quint32 row = 0; row < model.rowCount(); ++row)
             {
                for(quint32 column = 0; column < model.columnCount(); ++column)
                {
                    socketStream << model.record(row).value(column);
                }
             }
             }else
             {
                 QString msg = "employee info is empty!!!";
                 emit newMessage(msg);
             }
        }
        else
            QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPServer","Not connected");
}
