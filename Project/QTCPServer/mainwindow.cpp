#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QSqlRecord>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  m_server = new QTcpServer();

  if (m_server->listen(QHostAddress::Any, 8080)) // start server
  {
    connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
    connect(m_server, &QTcpServer::newConnection, this,
            &MainWindow::newConnection);
    ui->statusBar->showMessage("Server is listening...");
  } else {
    QMessageBox::critical(this, "QTCPServer",
                          QString("Unable to start the server: %1.")
                              .arg(m_server->errorString()));
    exit(EXIT_FAILURE);
  }

  if (dbUtils.connectToDB()) {

    displayMessage("Connected to database " + dbUtils.dbName + " sucessufully");
  } else {
    displayMessage("Connected to database " + dbUtils.dbName + " failure");
  }
}

MainWindow::~MainWindow() {
  for (QMap<QTcpSocket *, QString>::Iterator it = setOfConnectionUser.begin();
       it != setOfConnectionUser.end(); ++it) {
    it.key()->close();
    it.key()->deleteLater();
    dbUtils.dropUser(it.value());
  }

  m_server->close();
  m_server->deleteLater();
  dbUtils.closeDBConnection();

  delete ui;
}

void MainWindow::newConnection() {
  while (m_server->hasPendingConnections())
    appendToSocketList(m_server->nextPendingConnection());
}

void MainWindow::appendToSocketList(QTcpSocket *socket) {

  qDebug() << "Socket " << socket;
  // add socket to connection map , ("" - because user login unknown)
  setOfConnectionUser.insert(socket, "");

  connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
  connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);

  connect(socket, &QAbstractSocket::errorOccurred, this,
          &MainWindow::displayError);

  displayMessage(QString("INFO %2 :: Client (Name: %3,  Adress: %4, Port: %5) "
                         "with socketID:%1 has just entered the room")
                     .arg(socket->socketDescriptor())
                     .arg(QDateTime::currentDateTime().toString())
                     .arg(socket->peerName())
                     .arg(socket->peerAddress().toString())
                     .arg(socket->peerPort()));
}

void MainWindow::readSocket() {
  if (dbUtils.db.isOpen()) {
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
    // socketStream >> buffer;
    socketStream >> headCommand;

    // cast qbytearray to string
    // QString header(buffer);

    QString messageHd = QString("%1 ::<Received HEADER> -> %2")
                            .arg(socket->socketDescriptor())
                            .arg(headCommand);
    emit newMessage(messageHd);

    // autorization
    if (headCommand == msg::header::autorazation) {
      // read login
      socketStream >> buffer;
      QString login(buffer);

      // read password
      socketStream >> buffer;
      QString password(buffer);
      // send to client status of autorization
      sendAutorizationStatus(socket, dbUtils.checkUserIsExist(login, password),
                             login);
    } else {
      // read user unique id
      socketStream >> userUniqueID;
      emit newMessage(QString("Unique ID %1").arg(userUniqueID));
      qDebug() << "Unique ID " << userUniqueID;
      // check if unique ud is exist in db
      if (!dbUtils.checkUniqueId(userUniqueID)) {
        // send that user is not autirizated
        sendUnAutorizedStatus(socket);
        // skip all condition
        headCommand = ~0;
        // return;
      };
      /*employee info*/
      if (headCommand == msg::header::employeeDetail) {
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
        sendInitialData(socket, *employeeDetail);
      }

      /*department info*/
      else if (headCommand == msg::header::infoDemp) {
        sendEmpDept(socket);
      }
      /*designation info*/
      else if (headCommand == msg::header::infoDesig) {

        QVariant deptName{};
        socketStream >> deptName;
        emit newMessage("Get deptName -> " + deptName.toString());
        auto converted = deptName.toString();
        sendEmpDesig(socket, converted);

      } else if (headCommand == msg::id) {

        QVariant demp{};
        QVariant desig{};
        socketStream >> demp >> desig;
        emit newMessage(QString("Received demp in ID -> %1 and desig -> %2")
                            .arg(demp.toString(), desig.toString()));

        QString temp = demp.toString();
        QString temp2 = desig.toString();
        sendEmployeeID(socket, temp, temp2);
      } else if (headCommand == msg::salary) {

        QVariant demp{};
        QVariant desig{};
        socketStream >> demp >> desig;
        emit newMessage(QString("Received demp in salary -> %1 and desig -> %2")
                            .arg(demp.toString(), desig.toString()));
        QString temp = demp.toString();
        QString temp2 = desig.toString();
        sendSalary(socket, temp, temp2);
      } else if (msg::header::addEmployee) {
        QVariantList list{};
        for (int i = 0; i < 13; i++) {
          QVariant info{};
          socketStream >> info;
          list.append(info);
        }

        addEmployeeToDB(socket, list);
      }

      else {
        QString message =
            &"Received header does not much any of existing header: "
                [headCommand];
        emit newMessage(message);
        return;
      }
    }
    if (!socketStream.commitTransaction()) {
      QString message =
          QString("%1 :: Waiting for more data to come.. after all condition")
              .arg(socket->socketDescriptor());
      emit newMessage(message);
      QString smth(buffer);
      emit newMessage("Comming data: --> " + smth);
      return;
    }

  } else {
    QString message = QString("Data base is could not open");
    emit newMessage(message);
  }
}

void MainWindow::discardSocket() {
  QTcpSocket *socket = reinterpret_cast<QTcpSocket *>(sender());
  qDebug() << "Socket in discsrd = " << socket;

  // debug
  // for (auto &i : qAsConst(connection_set)) {
  //  qDebug() << "Socket in list" << i->socketDescriptor();
  // }

  // QSet<QTcpSocket *>::iterator it = connection_set.find(socket);

  // qDebug() << "Socket " << *it;

  // delete socket
  //  if (it != connection_set.end())
  //  {
  //    displayMessage(QString(" INFO %1 :: A client has just left the room")
  //                       .arg(socket->socketDescriptor()));
  //    connection_set.remove(*it);
  //  }

  QMap<QTcpSocket *, QString>::iterator myIt = setOfConnectionUser.find(socket);
  if (myIt != setOfConnectionUser.end()) {
    displayMessage(QString(" INFO %1 :: A client has just left the room")
                       .arg(myIt.value()));
    dbUtils.dropUser(myIt.value());

    setOfConnectionUser.remove(myIt.key());
  }

  socket->deleteLater();
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
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QMessageBox::information(this, "QTCPServer",
                             QString("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    break;
  }
}

void MainWindow::sendAutorizationStatus(QTcpSocket *socket, bool isAutorized,
                                        QString &login) {
  if (socket) {
    if (socket->isOpen()) {
      QDataStream socketStream(socket);
      socketStream.setVersion(QDataStream::Qt_5_15);
      if (isAutorized) {
        qDebug() << "Is admin? with login" << login
                 << dbUtils.checkUserIsAutorized(login);
        if (dbUtils.checkUserIsAutorized(login)) {
          socketStream << msg::header::autorazation
                       << st.alreadyAutorized.toUtf8();
        } else {
          auto randNum = this->getUniqueNum();

          if (dbUtils.insertUserToAutorizedTable(login, randNum,
                                                 socket->socketDescriptor())) {
            // send autorization success
            socketStream << msg::header::autorazation << st.success.toUtf8()
                         << randNum;

            auto mySet = setOfConnectionUser.find(socket);
            if (mySet != setOfConnectionUser.end()) {
              setOfConnectionUser[mySet.key()] = login;
            }

          } else {
            socketStream << msg::header::autorazation
                         << st.unknownError.toUtf8();
          }
        }

      } else {
        // send autorization unsuccess
        socketStream << msg::header::autorazation << st.failure.toUtf8();
      }
    } else
      QMessageBox::critical(this, "QTCPServer",
                            "Socket doesn't seem to be opened");
  } else
    QMessageBox::critical(this, "QTCPServer", "Not connected");
}

void MainWindow::sendEmpDept(QTcpSocket *socket) {
  if (socket) {
    if (socket->isOpen()) {
      QDataStream socketStream(socket);
      socketStream.setVersion(QDataStream::Qt_5_15);

      auto empDept = dbUtils.getDepartmentList();
      for (int i = 0; i < empDept->rowCount(); ++i) {
        emit newMessage("Dept item ->  " +
                        empDept->record(i).value(0).toString().toUtf8());
        qDebug() << "| " << quint16(empDept->columnCount()) << "| "
                 << quint16(empDept->rowCount());
      }
      emit newMessage(&": row in dept -> "[(empDept->columnCount())]);
      if (empDept->rowCount() > 0) {

        socketStream << msg::header::infoDemp << qint32(empDept->rowCount());

        for (int i = 0; i < empDept->rowCount(); ++i) {
          socketStream << empDept->record(i).value(0).toString().toUtf8();
        }
      } else {
        socketStream << msg::header::infoDemp << (empDept->rowCount());
      }

    } else
      QMessageBox::critical(this, "QTCPServer",
                            "Socket doesn't seem to be opened");
  } else
    QMessageBox::critical(this, "QTCPServer", "Not connected");
}

void MainWindow::sendEmpDesig(QTcpSocket *socket, QString &deptName) {
  if (!socket) {
    QMessageBox::critical(this, "QTCPServer", "Not connected");
    return;
  }

  if (!socket->isOpen()) {
    QMessageBox::critical(this, "QTCPServer",
                          "Socket doesn't seem to be opened");
    return;
  }

  QDataStream socketStream(socket);
  socketStream.setVersion(QDataStream::Qt_5_15);
  auto desig = dbUtils.getDesignationList(deptName);

  for (int i = 0; i < desig->rowCount(); ++i) {
    emit newMessage("Dept item ->  " +
                    desig->record(i).value(0).toString().toUtf8());
    qDebug() << "| " << quint16(desig->columnCount()) << "| "
             << quint16(desig->rowCount());
  }
  emit newMessage(&": row in desig -> "[(desig->columnCount())]);

  if (desig->rowCount() > 0) {

    socketStream << msg::header::infoDesig << qint32(desig->rowCount());

    for (int i = 0; i < desig->rowCount(); ++i) {
      socketStream << desig->record(i).value(0).toString().toUtf8();
    }
  } else {
    socketStream << msg::header::infoDesig << (qint32(desig->rowCount()));
  }
}

void MainWindow::sendUnAutorizedStatus(QTcpSocket *socket) {
  if (socket) {
    if (socket->isOpen()) {
      QDataStream socketStream(socket);
      socketStream.setVersion(QDataStream::Qt_5_15);
      socketStream << msg::unAutorizate;

    } else
      QMessageBox::critical(this, "QTCPServer",
                            "Socket doesn't seem to be opened");

  } else
    QMessageBox::critical(this, "QTCPServer", "Not connected");
}

void MainWindow::sendSalary(QTcpSocket *socket, QString &empDemt,
                            QString &empDesig) {
  if (!socket) {
    QMessageBox::critical(this, "QTCPServer", "Not connected");
    return;
  }

  if (!socket->isOpen()) {
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
                                QString &empDesig) {
  if (!socket) {
    QMessageBox::critical(this, "QTCPServer", "Not connected");
    return;
  }

  if (!socket->isOpen()) {
    QMessageBox::critical(this, "QTCPServer",
                          "Socket doesn't seem to be opened");
    return;
  }

  QDataStream socketStream(socket);
  socketStream.setVersion(QDataStream::Qt_5_15);

  auto empId = dbUtils.getEmployeeID(dbUtils.getDepartmentShortName(empDept),
                                     dbUtils.getDesignationShortName(empDesig));

  socketStream << msg::header::id << empId;
}

void MainWindow::addEmployeeToDB(QTcpSocket *socket,
                                 QVariantList &employeeInfo) {
  if (!socket) {
    QMessageBox::critical(this, "QTCPServer", "Not connected");
    return;
  }

  if (!socket->isOpen()) {
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

quint64 MainWindow::getUniqueNum() {
  QRandomGenerator generator;
  quint64 rand = generator.generate() & std::numeric_limits<qint64>::max();
  emit displayMessage(QString("The rand num generate is  %1").arg(rand));
  return rand;
}

void MainWindow::displayMessage(const QString &str) {
  ui->textBrowser_receivedMessages->append(str);
}

void MainWindow::sendTotalInfoEmployee(QTcpSocket *socket, int &lastID,
                                       QString &dept, QString &designTtl) {
  if (socket) {
    if (socket->isOpen()) {
      QDataStream socketStream(socket);
      socketStream.setVersion(QDataStream::Qt_6_3);

      // send header , last ID, dept, desighTotal
      socketStream << msg::header::totalInfoEmployee << quint32(lastID)
                   << dept.toUtf8() << designTtl.toUtf8();
      //
      emit newMessage(&"Sending header -> "[msg::header::totalInfoEmployee]);
    } else
      QMessageBox::critical(this, "QTCPServer",
                            "Socket doesn't seem to be opened");
  } else
    QMessageBox::critical(this, "QTCPServer", "Not connected");
}

void MainWindow::sendInitialData(QTcpSocket *socket, QSqlQueryModel &model) {
  if (socket) {
    if (socket->isOpen()) {
      QDataStream socketStream(socket);
      socketStream.setVersion(QDataStream::Qt_5_15);

      //  packet -> \header\row_count\column_count\
      // count row and column
      socketStream << msg::header::employeeDetail << quint32(model.rowCount())
                   << quint32(model.columnCount());

      if (model.rowCount() > 0 && model.columnCount() > 0) {
        // send data employee
        for (quint32 row = 0; row < model.rowCount(); ++row) {
          for (quint32 column = 0; column < model.columnCount(); ++column) {
            socketStream << model.record(row).value(column);
          }
        }
      } else {
        QString msg = "employee info is empty!!!";
        emit newMessage(msg);
      }
    } else
      QMessageBox::critical(this, "QTCPServer",
                            "Socket doesn't seem to be opened");
  } else
    QMessageBox::critical(this, "QTCPServer", "Not connected");
}
