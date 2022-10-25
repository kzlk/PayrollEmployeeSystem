#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_server = new QTcpServer();
    // LocalHostIPv6 for network
    if (m_server->listen(QHostAddress::LocalHost, 5400)) // start server
    {
        connect(this, &MainWindow::newMessage, this,
                &MainWindow::displayMessage);
        connect(m_server, &QTcpServer::newConnection, this,
                &MainWindow::newConnection);
        ui->statusBar->showMessage("Server is listening...");
    }
    else
    {
        QMessageBox::critical(this, "QTCPServer",
                              QString("Unable to start the server: %1.")
                                  .arg(m_server->errorString()));
        exit(EXIT_FAILURE);
    }

    if (dbUtils.connectToDB())
    {

        displayMessage("Connected to database " + dbUtils.dbName +
                       " sucessufully");

        /*Create AutoPilot*/
        myAutoPilot = new CAutoPilot(1000, dbUtils);
        myAutoPilot->start();
    }
    else
    {

        displayMessage("Connected to database " + dbUtils.dbName + " failure");
    }
}

MainWindow::~MainWindow()
{

    for (QMap<QTcpSocket *, QString>::Iterator it = setOfConnectionUser.begin();
         it != setOfConnectionUser.end();)
    {
        disconnect(it.key(), &QTcpSocket::disconnected, this,
                   &MainWindow::discardSocket);
        dbUtils.dropUser(it.value());
        it.key()->close();
        it.key()->deleteLater();
        setOfConnectionUser.erase(it++);
    }
    setOfConnectionUser.clear();

    m_server->close();
    m_server->deleteLater();
    dbUtils.closeDBConnection();

    delete ui;
}

void MainWindow::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

void MainWindow::appendToSocketList(QTcpSocket *socket)
{

    qDebug() << "Socket " << socket;
    // add socket to connection map , ("" - because user login unknown)
    setOfConnectionUser.insert(socket, "");

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this,
            &MainWindow::discardSocket);

    connect(socket, &QAbstractSocket::errorOccurred, this,
            &MainWindow::displayError);

    displayMessage(
        QString("INFO %2 :: Client (Name: %3,  Adress: %4, Port: %5) "
                "with socketID:%1 has just entered the room")
            .arg(socket->socketDescriptor())
            .arg(QDateTime::currentDateTime().toString())
            .arg(socket->peerName())
            .arg(socket->peerAddress().toString())
            .arg(socket->peerPort()));
}

void MainWindow::readSocket()
{
    if (dbUtils.db.isOpen())
    {
        QTcpSocket *socket = reinterpret_cast<QTcpSocket *>(sender());

        QByteArray buffer;
        buffer.clear();

        QDataStream socketStream(socket);
        socketStream.setVersion(QDataStream::Qt_5_15);

        // start transaction for reading data from socket
        socketStream.startTransaction();

        // read header(comand) that will be executed
        quint8 headCommand{};
        quint64 userUniqueID{};

        socketStream >> headCommand;

        QString messageHd = QString("%1 ::<Received HEADER> -> %2")
                                .arg(socket->socketDescriptor())
                                .arg(headCommand);
        emit newMessage(messageHd);

        // autorization
        if (headCommand == msg::header::autorazation)
        {
            // read login
            socketStream >> buffer;
            QString login(buffer);

            // read password
            socketStream >> buffer;
            QString password(buffer);
            // send to client status of autorization
            sendAutorizationStatus(
                socket, dbUtils.checkUserIsExist(login, password), login);
        }

        else if (headCommand == msg::header::getEmployeeNameIdSurname)
        {
            sendNameIdSurname(socket, *dbUtils.getNameIdSurname());
            headCommand = ~0;
        }
        else if (headCommand == msg::header::getAttandanceDate)
        {
            QString Id{};
            socketStream >> Id;
            sendAttamdanceInfo(socket, Id);
        }
        else if (headCommand == msg::header::setEmployeeAttandanceExit)
        {
            QString Id{};
            QDateTime rec{};
            socketStream >> Id >> rec;
            emit newMessage("Received from Scanner ID Exit " + Id +
                            " And time " + rec.toString());
            this->sendStatusUpdateExit(socket, rec, Id);
        }
        else if (headCommand == msg::header::setEmployeeAttandanceEntered)
        {
            QString Id{};
            QDateTime rec{};
            socketStream >> Id >> rec;
            emit newMessage("Received from Scanner ID Entered: " + Id +
                            " And time " + rec.toString());
            this->sendStatusInsertEntered(socket, rec, Id);
        }
        else
        {
            // read user unique id
            socketStream >> userUniqueID;
            emit newMessage(QString("Unique ID %1").arg(userUniqueID));
            qDebug() << "Unique ID " << userUniqueID;
            // check if unique ud is exist in db
            if (!dbUtils.checkUniqueId(userUniqueID))
            {
                // send that user is not autirizated
                sendUnAutorizedStatus(socket);
                // skip all condition
                headCommand = ~0;
                // return;
            };
            /*employee info*/
            if (headCommand == msg::header::employeeDetail)
            {
                // total employee
                auto lastID = dbUtils.getTotalEmployee();
                int totalEmp{};
                if (lastID != "")
                    totalEmp = lastID.toInt();

                QString dept = dbUtils.getTotalDept();

                QString designTtl = dbUtils.getTotalDesign();

                // send total info of emloyee
                sendTotalInfoEmployee(socket, totalEmp, dept, designTtl);

                auto employeeDetail = dbUtils.getEmployeeDetails();

                // send employee info
                sendInitialData(msg::header::employeeDetail, socket,
                                *employeeDetail);
            }

            /*department info*/
            else if (headCommand == msg::header::infoDemp)
            {
                sendEmpDept(socket);
            }
            /*designation info*/
            else if (headCommand == msg::header::infoDesig)
            {

                QVariant deptName{};
                socketStream >> deptName;
                emit newMessage("Get deptName -> " + deptName.toString());
                auto converted = deptName.toString();
                sendEmpDesig(socket, converted);
            }
            else if (headCommand == msg::id)
            {

                QVariant demp{};
                QVariant desig{};
                socketStream >> demp >> desig;
                emit newMessage(
                    QString("Received demp in ID -> %1 and desig -> %2")
                        .arg(demp.toString(), desig.toString()));

                QString temp = demp.toString();
                QString temp2 = desig.toString();
                sendEmployeeID(socket, temp, temp2);
            }
            else if (headCommand == msg::salary)
            {

                QVariant demp{};
                QVariant desig{};
                socketStream >> demp >> desig;
                emit newMessage(
                    QString("Received demp in salary -> %1 and desig -> %2")
                        .arg(demp.toString(), desig.toString()));
                QString temp = demp.toString();
                QString temp2 = desig.toString();
                sendSalary(socket, temp, temp2);
            }
            else if (headCommand == msg::header::addEmployee)
            {
                QVariantList list{};
                for (int i = 0; i < 13; i++)
                {
                    QVariant info{};
                    socketStream >> info;
                    list.append(info);
                }

                addEmployeeToDB(socket, list);
            }
            else if (headCommand == msg::header::insertInDBSettingPayment)
            {
                // read data
                // insert in db
                // send to server status
                QVariant start;
                QVariant next;
                QVariant autop;
                QVariant freq;
                QVariant end;

                socketStream >> start >> end >> next >> autop >> freq;

                sendStatusInsertedInSetting(socket, start.toDateTime(),
                                            end.toDateTime(), next.toDateTime(),
                                            autop.toInt(), freq.toInt());
            }
            else if (headCommand == msg::header::getSettingsData)
            {
                sendSettingInfo(socket);
            }
            else if (headCommand == msg::header::getReportPeriodInfo)
            {
                sendPaymentPeriodData(socket);
            }
            else if (headCommand == msg::header::getDetailInfoPaymentPeriod)
            {
                QVariant id{};
                socketStream >> id;
                emit newMessage(
                    QString("The id payment received is %1").arg(id.toInt()));
                sendPaymentInfoDetail(socket, id.toInt());
            }
            else if (headCommand == msg::header::getPdfData)
            {
                QVariant id{};
                socketStream >> id;
                sendDataForPdfReport(socket, id.toString());
            }
            else if (headCommand == msg::header::getSearchedEmployee)
            {
                QVariant searchedText{};
                socketStream >> searchedText;
                sendSearchedEmployee(socket, searchedText.toString());
            }
            else if (headCommand == msg::header::getEmployeeInfo)
            {
                QVariant id{};
                socketStream >> id;
                sendDataEmpDetail(socket, id.toString());
            }
            else if (headCommand == msg::header::deleteEmployee)
            {
                auto employeeDetail = dbUtils.getEmployeeDetails();
                sendInitialData(msg::header::deleteEmployee, socket,
                                *employeeDetail);
            }
            else if (headCommand == msg::header::deleteInfoEmp)
            {
                QVariant id{};
                socketStream >> id;
                deleteEmployee(socket, id.toString());
            }
            else if (headCommand == msg::updateEployee)
            {
                sendListForUpdatePage(socket);
            }
            else if (headCommand == msg::getOneEmpForUpdate)
            {

                QVariant id{};
                socketStream >> id;
                sendSelectedEmpForUpdatePage(socket, id.toString());
            }
            else if (headCommand == msg::setUpdateOneEmployee)
            {
                setNewDataForEmployee(socket, socketStream);
            }
            else
            {
                QString message =
                    &"Received header does not much any of existing header: "
                        [headCommand];
                emit newMessage(message);
                return;
            }
        }
        if (!socketStream.commitTransaction())
        {
            QString message =
                QString(
                    "%1 :: Waiting for more data to come.. after all condition")
                    .arg(socket->socketDescriptor());
            emit newMessage(message);
            QString smth(buffer);
            emit newMessage("Comming data: --> " + smth);
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
    QTcpSocket *socket = reinterpret_cast<QTcpSocket *>(sender());
    qDebug() << "Socket in discsrd = " << socket;

    QMap<QTcpSocket *, QString>::iterator myIt =
        setOfConnectionUser.find(socket);
    if (myIt != setOfConnectionUser.end())
    {
        displayMessage(QString(" INFO %1 :: A client has just left the room")
                           .arg(myIt.value()));
        /*Drop user from db*/
        dbUtils.dropUser(myIt.value());
        qDebug() << "After drop user in table" << *myIt;
        setOfConnectionUser.remove(myIt.key());
        // setOfConnectionUser.erase(myIt);
    }

    socket->deleteLater();
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(
            this, "QTCPServer",
            "The host was not found. Please check the host "
            "name and port settings.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(
            this, "QTCPServer",
            "The connection was refused by the peer. Make sure QTCPServer is "
            "running, and check that the host name and port settings are "
            "correct.");
        break;
    default:
        QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
        QMessageBox::information(this, "QTCPServer",
                                 QString("The following error occurred: %1.")
                                     .arg(socket->errorString()));
        break;
    }
}

void MainWindow::sendAutorizationStatus(QTcpSocket *socket, bool isAutorized,
                                        QString &login)
{
    if (socket)
    {
        if (socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);
            if (isAutorized)
            {
                qDebug() << "Is admin? with login" << login
                         << dbUtils.checkUserIsAutorized(login);
                if (dbUtils.checkUserIsAutorized(login))
                {
                    socketStream << msg::header::autorazation
                                 << st.alreadyAutorized.toUtf8();
                }
                else
                {
                    auto randNum = this->getUniqueNum();

                    if (dbUtils.insertUserToAutorizedTable(
                            login, randNum, socket->socketDescriptor()))
                    {
                        // send autorization success
                        socketStream << msg::header::autorazation
                                     << st.success.toUtf8() << randNum;

                        auto mySet = setOfConnectionUser.find(socket);
                        if (mySet != setOfConnectionUser.end())
                        {
                            setOfConnectionUser[mySet.key()] = login;
                        }
                    }
                    else
                    {
                        socketStream << msg::header::autorazation
                                     << st.unknownError.toUtf8();
                    }
                }
            }
            else
            {
                // send autorization unsuccess
                socketStream << msg::header::autorazation
                             << st.failure.toUtf8();
            }
        }
        else
            QMessageBox::critical(this, "QTCPServer",
                                  "Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this, "QTCPServer", "Not connected");
}

void MainWindow::sendEmpDept(QTcpSocket *socket)
{
    if (socket)
    {
        if (socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            auto empDept = dbUtils.getDepartmentList();
            for (int i = 0; i < empDept->rowCount(); ++i)
            {
                emit newMessage(
                    "Dept item ->  " +
                    empDept->record(i).value(0).toString().toUtf8());
                qDebug() << "| " << quint16(empDept->columnCount()) << "| "
                         << quint16(empDept->rowCount());
            }
            emit newMessage(&": row in dept -> "[(empDept->columnCount())]);
            if (empDept->rowCount() > 0)
            {

                socketStream << msg::header::infoDemp
                             << qint32(empDept->rowCount());

                for (int i = 0; i < empDept->rowCount(); ++i)
                {
                    socketStream
                        << empDept->record(i).value(0).toString().toUtf8();
                }
            }
            else
            {
                socketStream << msg::header::infoDemp << (empDept->rowCount());
            }
        }
        else
            QMessageBox::critical(this, "QTCPServer",
                                  "Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this, "QTCPServer", "Not connected");
}

void MainWindow::sendEmpDesig(QTcpSocket *socket, QString &deptName)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);
    auto desig = dbUtils.getDesignationList(deptName);

    for (int i = 0; i < desig->rowCount(); ++i)
    {
        emit newMessage("Dept item ->  " +
                        desig->record(i).value(0).toString().toUtf8());
        qDebug() << "| " << quint16(desig->columnCount()) << "| "
                 << quint16(desig->rowCount());
    }
    emit newMessage(&": row in desig -> "[(desig->columnCount())]);

    if (desig->rowCount() > 0)
    {

        socketStream << msg::header::infoDesig << qint32(desig->rowCount());

        for (int i = 0; i < desig->rowCount(); ++i)
        {
            socketStream << desig->record(i).value(0).toString().toUtf8();
        }
    }
    else
    {
        socketStream << msg::header::infoDesig << (qint32(desig->rowCount()));
    }
}

void MainWindow::sendUnAutorizedStatus(QTcpSocket *socket)
{
    if (socket)
    {
        if (socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);
            socketStream << msg::unAutorizate;
        }
        else
            QMessageBox::critical(this, "QTCPServer",
                                  "Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this, "QTCPServer", "Not connected");
}

void MainWindow::sendSalary(QTcpSocket *socket, QString &empDemt,
                            QString &empDesig)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto salary = dbUtils.getSalary(empDemt, empDesig);

    socketStream << msg::salary << salary;
}

void MainWindow::sendEmployeeID(QTcpSocket *socket, QString &empDept,
                                QString &empDesig)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto empId =
        dbUtils.getEmployeeID(dbUtils.getDepartmentShortName(empDept),
                              dbUtils.getDesignationShortName(empDesig));

    socketStream << msg::header::id << empId;
}

void MainWindow::addEmployeeToDB(QTcpSocket *socket, QVariantList &employeeInfo)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    if (dbUtils.addEmployee(employeeInfo))
        socketStream << msg::header::addEmployee << st.success;
    else
        socketStream << msg::header::addEmployee << st.failure;
}

void MainWindow::sendNameIdSurname(QTcpSocket *socket, QSqlQueryModel &model)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    //  packet -> \header\row_count\column_count\
    // count row and column
    socketStream << msg::header::getEmployeeNameIdSurname
                 << quint32(model.rowCount()) << quint32(model.columnCount());

    if (model.rowCount() > 0 && model.columnCount() > 0)
    {
        // send data employee
        for (quint32 row = 0; row < model.rowCount(); ++row)
        {
            for (quint32 column = 0; column < model.columnCount(); ++column)
            {
                socketStream << model.record(row).value(column);
            }
        }
    }
    else
    {
        emit newMessage("EmployeeNameIdSurname info is empty!!!");
    }
}

void MainWindow::sendAttamdanceInfo(QTcpSocket *socket, QString &Id)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto a = dbUtils.getAttandanceInfo(Id);
    if (a->rowCount() == 0)
    {
        socketStream << msg::header::getAttandanceDate << st.empty;
    }

    else
    {
        socketStream << msg::header::getAttandanceDate
                     << a->record(0).value(0).toDateTime().toString();
    }
}

void MainWindow::sendStatusInsertEntered(QTcpSocket *socket,
                                         QDateTime &enterDate, QString &empId)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    if (dbUtils.insertEnteredAttandance(enterDate, empId))
    {
        socketStream << msg::setAttandanceSuccess;
    }
    else
    {
        socketStream << msg::setAttandanceFailure;
    }
}

void MainWindow::sendStatusUpdateExit(QTcpSocket *socket, QDateTime &exitDate,
                                      QString &empId)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    if (dbUtils.updateExitAttandance(exitDate, empId))
    {
        socketStream << msg::setAttandanceSuccess;
    }
    else
    {
        socketStream << msg::setAttandanceFailure;
    }
}

void MainWindow::sendStatusInsertedInSetting(QTcpSocket *socket,
                                             QDateTime startPeriod,
                                             QDateTime endPeriod,
                                             QDateTime nextPayment,
                                             qint8 autopilot, qint8 frequency)
{

    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto isOldAutoPilot = dbUtils.checkIfAutoPilotIsOn();

    if (dbUtils.insertIntoSettingTable(startPeriod, endPeriod, nextPayment,
                                       autopilot, frequency))
    {

        socketStream << msg::header::insertedInDBSettingStatus << st.success;
        /*Start autopilot if it was disable and now user activate it*/
        if (!isOldAutoPilot && autopilot == 1)
            myAutoPilot->start();
        else if (isOldAutoPilot && autopilot == 0)
            myAutoPilot->stop();
    }
    else
    {
        socketStream << msg::header::insertedInDBSettingStatus << st.failure;
    }
}

void MainWindow::sendSettingInfo(QTcpSocket *socket)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto dataFromSysTable = dbUtils.getDataFromSystemSetting();

    if (dataFromSysTable->rowCount() < 1)
    {
        socketStream << msg::header::getSettingsData << st.empty;
    }

    else
    {
        socketStream << msg::header::getSettingsData << st.success
                     << dataFromSysTable->record(0).value(0) // start
                     << dataFromSysTable->record(0).value(2) // end
                     << dataFromSysTable->record(0).value(1) // next
                     << dataFromSysTable->record(0).value(3) // freq
                     << dbUtils.checkIfAutoPilotIsOn();      // autopilot
    }
}

void MainWindow::sendPaymentPeriodData(QTcpSocket *socket)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto dataFromPeriodTable = dbUtils.getPaymentPeriodData();

    //  packet -> |header|row_count|column_count|
    // count row and column
    socketStream << msg::header::getReportPeriodInfo
                 << quint32(dataFromPeriodTable->rowCount())
                 << quint32(dataFromPeriodTable->columnCount());

    if (dataFromPeriodTable->rowCount() > 0 &&
        dataFromPeriodTable->columnCount() > 0)
    {
        // send data employee (pay_id,start,end, total_net,total_cnt_emp )
        for (quint32 row = 0; row < dataFromPeriodTable->rowCount(); ++row)
        {
            for (quint32 column = 0;
                 column < dataFromPeriodTable->columnCount(); ++column)
            {
                socketStream << dataFromPeriodTable->record(row).value(column);
            }
        }
    }

    /*
        socketStream << msg::header::getSettingsData
                     << dataFromPeriodTable->record(0).value(0) // pay_id
                     << dataFromPeriodTable->record(0).value(2) // start
                     << dataFromPeriodTable->record(0).value(1) // end
                     << dataFromPeriodTable->record(0).value(3) // total_net
                     << dataFromPeriodTable->record(0).value(4);  //
       total_cnt_emp
     */
}

void MainWindow::sendPaymentInfoDetail(QTcpSocket *socket, int id)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto dataFromPeriodTable = dbUtils.getDataDetailPaymentTable(id);

    //  packet -> |header|row_count|column_count|
    // count row and column
    socketStream << msg::header::getDetailInfoPaymentPeriod
                 << quint32(dataFromPeriodTable->rowCount())
                 << quint32(dataFromPeriodTable->columnCount());

    if (dataFromPeriodTable->rowCount() > 0 &&
        dataFromPeriodTable->columnCount() > 0)
    {
        // send data employee (pay_id,start,end, total_net,total_cnt_emp )
        for (quint32 row = 0; row < dataFromPeriodTable->rowCount(); ++row)
        {
            for (quint32 column = 0;
                 column < dataFromPeriodTable->columnCount(); ++column)
            {
                socketStream << dataFromPeriodTable->record(row).value(column);
            }
        }
    }
}

void MainWindow::sendDataForPdfReport(QTcpSocket *socket, QString empId)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto dataFromPeriodTable = dbUtils.getPaymentDataForPDFGenerate(empId);

    QVariantList pdfData{};
    if (dataFromPeriodTable->rowCount() > 0 &&
        dataFromPeriodTable->columnCount() > 0)
    {
        for (quint32 row = 0; row < dataFromPeriodTable->rowCount(); ++row)
        {
            for (quint32 column = 0;
                 column < dataFromPeriodTable->columnCount(); ++column)
            {
                pdfData.push_back(
                    dataFromPeriodTable->record(row).value(column));
            }
        }

        auto res = CGeneratePdf::generateReportInfo(pdfData);

        if (!res.isEmpty())
        {
            socketStream << msg::header::getPdfData << st.success
                         << res.toUtf8();
            // qDebug() << res;
            emit newMessage(res);
        }

        return;
    }
    else
    {
        socketStream << msg::header::getPdfData << st.failure;
    }
}

void MainWindow::sendSearchedEmployee(QTcpSocket *socket, QString searchedText)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto model = dbUtils.searchEmployeeDetails(searchedText);

    socketStream << msg::header::getSearchedEmployee
                 << quint32(model->rowCount()) << quint32(model->columnCount());

    if (model->rowCount() > 0 && model->columnCount() > 0)
    {
        // send data employee
        for (quint32 row = 0; row < model->rowCount(); ++row)
        {
            for (quint32 column = 0; column < model->columnCount(); ++column)
            {
                socketStream << model->record(row).value(column);
            }
        }
    }
    else
    {
        QString msg =
            "employee info with searched text " + searchedText + " is empty!!!";
        emit newMessage(msg);
    }
}

void MainWindow::sendDataEmpDetail(QTcpSocket *socket, QString empId)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);
    auto model = dbUtils.getData(empId);

    QVariantList empData{};

    if (model->rowCount() > 0 && model->columnCount() > 0)
    {
        // send data employee
        for (quint32 row = 0; row < model->rowCount(); ++row)
        {
            for (quint32 column = 0; column < model->columnCount(); ++column)
            {
                if (column != 0 && column != 1)
                    empData.push_back(model->record(row).value(column));
            }
        }

        socketStream << msg::header::getEmployeeInfo << st.success << empData;

        return;
    }
    else
    {
        socketStream << msg::header::getEmployeeInfo << st.failure;
        QString msg =
            "employee info with searched id " + empId + " is empty!!!";
        emit newMessage(msg);
    }
}

void MainWindow::deleteEmployee(QTcpSocket *socket, QString empId)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    if (dbUtils.deleteEmployeeRecord(empId))
    {
        socketStream << msg::header::deleteInfoEmp << st.success;
    }
    else
    {
        socketStream << msg::header::deleteInfoEmp << st.failure;
    }
}

void MainWindow::sendListForUpdatePage(QTcpSocket *socket)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    auto model = dbUtils.setEmployeeUpdateDetails();

    //  packet -> \header\row_count\column_count\
    //  count row and column

    socketStream << msg::header::updateEployee << quint32(model->rowCount())
                 << quint32(model->columnCount());

    if (model->rowCount() > 0 && model->columnCount() > 0)
    {
        // send data employee
        for (quint32 row = 0; row < model->rowCount(); ++row)
        {
            for (quint32 column = 0; column < model->columnCount(); ++column)
            {
                socketStream << model->record(row).value(column);
            }
        }
    }
    else
    {
        QString msg = "employee info is empty!!!";
        emit newMessage(msg);
    }
}

void MainWindow::sendSelectedEmpForUpdatePage(QTcpSocket *socket, QString empId)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);
    auto model = dbUtils.showEmployeeDetailsToLineEdit(empId);

    QVariantList empData{};

    if (model->rowCount() > 0 && model->columnCount() > 0)
    {
        // send data employee
        for (quint32 row = 0; row < model->rowCount(); ++row)
        {
            for (quint32 column = 0; column < model->columnCount(); ++column)
            {
                empData.push_back(model->record(row).value(column));
            }
        }

        socketStream << msg::header::getOneEmpForUpdate << st.success
                     << empData;

        return;
    }
    else
    {
        socketStream << msg::header::getOneEmpForUpdate << st.failure;
        QString msg =
            "employee info with searched id " + empId + " is empty!!!";
        emit newMessage(msg);
    }
}

void MainWindow::setNewDataForEmployee(QTcpSocket *socket,
                                       QDataStream &socketStream)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPServer", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCPServer",
                              "Socket doesn't seem to be opened");
        return;
    }
    QDataStream socketStream2(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);
    QVariantList list{};
    list.resize(8);

    for (int i = 0; i < 8; i++)
    {
        socketStream >> list[i];
        emit newMessage("The received update: " + list[i].toString());
    }

    if (dbUtils.setUpdate(list))
    {
        socketStream2 << msg::setUpdateOneEmployee << st.success;
    }
    else
    {
        socketStream2 << msg::setUpdateOneEmployee << st.failure;
    }
}

quint64 MainWindow::getUniqueNum()
{
    QRandomGenerator generator;
    quint64 rand = generator.generate() & std::numeric_limits<qint64>::max();
    emit displayMessage(QString("The rand num generate is  %1").arg(rand));
    return rand;
}

void MainWindow::displayMessage(const QString &str)
{
    ui->textBrowser_receivedMessages->append(str);
}

void MainWindow::sendTotalInfoEmployee(QTcpSocket *socket, int &lastID,
                                       QString &dept, QString &designTtl)
{
    if (socket)
    {
        if (socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_6_3);

            // send header , last ID, dept, desighTotal
            socketStream << msg::header::totalInfoEmployee << quint32(lastID)
                         << dept.toUtf8() << designTtl.toUtf8();
            //
            emit newMessage(
                &"Sending header -> "[msg::header::totalInfoEmployee]);
        }
        else
            QMessageBox::critical(this, "QTCPServer",
                                  "Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this, "QTCPServer", "Not connected");
}

void MainWindow::sendInitialData(msg::header header, QTcpSocket *socket,
                                 QSqlQueryModel &model)
{
    if (socket)
    {
        if (socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            //  packet -> \header\row_count\column_count\
      // count row and column
            socketStream << header << quint32(model.rowCount())
                         << quint32(model.columnCount());

            if (model.rowCount() > 0 && model.columnCount() > 0)
            {
                // send data employee
                for (quint32 row = 0; row < model.rowCount(); ++row)
                {
                    for (quint32 column = 0; column < model.columnCount();
                         ++column)
                    {
                        socketStream << model.record(row).value(column);
                    }
                }
            }
            else
            {
                QString msg = "employee info is empty!!!";
                emit newMessage(msg);
            }
        }
        else
            QMessageBox::critical(this, "QTCPServer",
                                  "Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this, "QTCPServer", "Not connected");
}
