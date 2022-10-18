#include "mainwindow.h"
#include "employeeinfo.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QDate>
#include <QMessageBox>
#include <QSqlQuery>
#include <QStringList>
#include <qapplication.h>

#include "about.h"
#include "techused.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // set style

  ui->empDOB->setCalendarPopup(true);
  ui->empDOJ->setCalendarPopup(true);
  ui->updateEmpDOB->setCalendarPopup(true);

  ui->tableWidget->setAlternatingRowColors(true);

  ui->tableWidget->setStyleSheet("alternate-background-color: #E4E4E4;"
                                 "background-color:white;"
                                 "selection-background-color:#1492E6;"
                                 "selection-color:white;");

  ui->updateTableView->setAlternatingRowColors(true);
  ui->updateTableView->setStyleSheet("alternate-background-color: "
                                     "#E4E4E4;background-color:white;"
                                     "selection-background-color:#1492E6;"
                                     "selection-color:white;");

  ui->deleteTableView->setAlternatingRowColors(true);
  ui->deleteTableView->setStyleSheet("alternate-background-color: "
                                     "#E4E4E4;background-color:white;"
                                     "selection-background-color:#1492E6;"
                                     "selection-color:white;");

  QObject::connect(this, &MainWindow::sendHeader,
                   &MainWindow::sendHeaderToServer);

  ui->searchStackedWidget->setCurrentWidget(0);
}

MainWindow::~MainWindow() {
  // emit sendHeader(msg::disconnect);
  delete ui;
}

// read data from server
void MainWindow::readSocket() {
  QByteArray buffer;
  buffer.clear();

  QDataStream socketStream(this->socket);

  socketStream.setVersion(QDataStream::Qt_6_3);

  // start socket transaction
  socketStream.startTransaction();

  // comand
  quint8 headCommand;
  // read header
  socketStream >> headCommand;
  if (headCommand == msg::unAutorizate) {
    QMessageBox::critical(this, "QTCPClient",
                          "You do not do any action because you are "
                          "unautorized user!\n Please log in!");
    headCommand = ~0;
  }

  // header to QString
  QString header(buffer);

  qDebug() << "\nRECEIVED HEADER " << headCommand << '\n';
  //
  if (headCommand == msg::totalInfoEmployee) {
    quint8 headerInfoEmp{};
    qint32 lastID{};
    QByteArray dept{};
    QByteArray design{};

    socketStream >> lastID >> dept >> design >> headerInfoEmp;
    qDebug() << "Data received --> " << headerInfoEmp << '\t' << lastID << '\t'
             << dept << '\t' << design;
    //
    loadTotalData(lastID, QString(dept), QString(design));

    if (headerInfoEmp == msg::employeeDetail) {
      //
      ui->searchStackedWidget->setCurrentWidget(0);
      //
      // dbUtils.setEmployeeDetails(ui->tableWidget, socketStream);
      quint32 row{};
      quint32 column{};

      socketStream >> row >> column;
      qDebug() << "\nRow received = " << row
               << " | Column received = " << column;
      ui->tableWidget->horizontalHeader()->setVisible(false);
      // ui->tableWidget->verticalHeader()->setVisible(false);
      ui->tableWidget->setShowGrid(false);
      ui->tableWidget->setStyleSheet(
          "QTableView {selection-background-color: red;}");
      ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
      ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
      ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

      //      ui->tableWidget->verticalHeader()->resizeSections(QHeaderView::Stretch);
      //      ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::Stretch);
      // ui->tableWidget->setGeometry(QApplication::primaryScreen()->geometry());
      //  ui->tableWidget->horizontalHeader()->setSectionResizeMode(
      //   QHeaderView::Stretch);
      // ui->tableWidget->verticalHeader()->setSectionResizeMode(
      // QHeaderView::ResizeToContents);
      // QHeaderView::Stretch,
      if (row > 0 && column > 0) {
        ui->tableWidget->setRowCount(row);
        ui->tableWidget->setColumnCount(column);
        ui->tableWidget->setWordWrap(true);
        ui->tableWidget->setColumnWidth(0, ui->label_8->width());
        ui->tableWidget->setColumnWidth(1, ui->label_21->width());
        ui->tableWidget->setColumnWidth(2, ui->label_22->width());
        ui->tableWidget->setColumnWidth(3, ui->label_23->width());
        ui->tableWidget->setColumnWidth(4, ui->label_24->width());
        ui->tableWidget->setColumnWidth(5, ui->label_25->width());

        ui->tableWidget->horizontalHeader()->setSectionResizeMode(
            QHeaderView::Stretch);
        ui->tableWidget->verticalHeader()->setSectionResizeMode(
            QHeaderView::Stretch);

        for (int r = 0; r < row; ++r) {
          // ui->tableWidget->insertRow(r);
          for (int col = 0; col < column; ++col) {

            QVariant receivedItem{};
            // QByteArray sItem{};
            socketStream >> receivedItem;
            qDebug() << receivedItem;
            QTableWidgetItem *poItem = new QTableWidgetItem();
            poItem->setData(Qt::DisplayRole, receivedItem);

            ui->tableWidget->setItem(r, col, poItem);
          }
        }
        // ui->tableWidget->show();
      }

    } else {
      // TODO: Error handling
      QMessageBox::information(
          this, "QTCPCLIENT",
          QString("header != %1 !").arg(msg::employeeDetail));
    }
  } else if (headCommand == msg::infoDemp) {
    ui->empDesig->clear();
    ui->searchStackedWidget->setCurrentIndex(3);
    QVector<QString> receiveDep{};
    buffer.clear();
    QByteArray size;
    qint32 rec{};
    size.clear();
    socketStream >> rec;
    qDebug() << "Count Row In " << headCommand << rec << '\n';
    if (rec > 0) {
      for (int i = 0; i < rec; ++i) {
        buffer.clear();
        socketStream >> buffer;
        receiveDep.emplaceBack(QString(buffer));
        qDebug() << " Row In " + header << receiveDep.at(i) << '\n';
      }
      // send header desig ()
      ui->empDept->clear();
      ui->empDept->addItems(receiveDep);
      //
      qDebug() << "From read socket " << ui->empDept->currentText();
      // emit sendHeader(msg::header::infoDesig, {ui->empDept->currentText()});
      // sendDempNameForDesign(ui->empDept->currentText());
    }
    // ui->empDesig->setModel(dbUtils.getDesignationList(ui->empDept));
  } else if (headCommand == msg::infoDesig) {

    QVector<QString> receiveDesig{};
    buffer.clear();
    qint32 rec{};
    socketStream >> rec;
    qDebug() << "Count Row In " << headCommand << rec << '\n';
    if (rec > 0) {
      for (int i = 0; i < rec; ++i) {
        buffer.clear();
        socketStream >> buffer;
        receiveDesig.emplaceBack(QString(buffer));
        qDebug() << " Row In " + header << receiveDesig.at(i) << '\n';
      }
      // send header desig ()
      ui->empDesig->clear();
      ui->empDesig->addItems(receiveDesig);
      //
    }

    // ui->empDesig->setModel(dbUtils.getDesignationList(ui->empDept));
  } else if (headCommand == msg::id) {
    QString id;
    socketStream >> id;
    qDebug() << "Received employee ID  ->   " << id;
    ui->empID->setText(id);

    emit sendHeader(msg::salary,
                    {ui->empDept->currentText(), ui->empDesig->currentText()});

  } else if (headCommand == msg::salary) {
    int salary{};
    socketStream >> salary;
    if (ui->empType->currentIndex() == 0) {
      salary -= 100;
      ui->empSalary->setText(QString::number(salary));
    } else {
      ui->empSalary->setText(QString::number(salary));
    }
  } else if (headCommand == msg::addEmployee) {
    QString status{};
    socketStream >> status;

    if (status == st.success) {
      QMessageBox::information(this, "Success", "Data inserted successfully");
    } else if (status == st.failure) {
      QMessageBox::critical(this, "Failure",
                            "Something went wrong when data inserted");
    }
  }

  else {
    // TODO: ERROR HANDLING
    QMessageBox::information(this, "QTCPCLIENT", "header != employeeDetail!");
  }

  if (!socketStream.commitTransaction()) {
    QString message =
        QString("%1 :: Waiting for more data to come..after all condition")
            .arg(socket->socketDescriptor());
    QMessageBox::information(this, "QTCPCLIENT", message);
    // emit newMessage(message);
    buffer.clear();
    // socketStream >> buffer;
    qDebug() << ("Comming data: --> " + QString(buffer));
    return;
  }
}

// set big three label on main window
void MainWindow::loadTotalData(int lastID, QString dept, QString designTtl) {
  ui->searchStackedWidget->setCurrentIndex(0);
  QString lastIDString = QString::number(lastID);

  if (lastIDString.length() == 1) {
    lastIDString = "00" + lastIDString;
  } else if (lastIDString.length() == 2) {
    lastIDString = "0" + lastIDString;
  }

  ui->empLabel->setText(lastIDString);

  if (dept.length() == 1) {
    dept = "00" + dept;
  } else if (dept.length() == 2) {
    dept = "0" + dept;
  }

  ui->deptLabel->setText(dept);

  if (designTtl.length() == 1) {
    designTtl = "00" + designTtl;
  } else if (designTtl.length() == 2) {
    designTtl = "0" + designTtl;
  }

  ui->designLabel->setText(designTtl);
}

// SET MOUSE EVENT
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

void MainWindow::on_pushButton_clicked() { ui->searchTextBox->setText(""); }

/*search button*/
void MainWindow::on_searchButton_clicked() {
  // send header to get employee info
  emit sendHeader(msg::employeeDetail);
  // loadTotalData();
  selectedPushButton(ui->searchButton);
  deselectedPushButton(ui->deleteEmpButton);
  deselectedPushButton(ui->techButton);
  deselectedPushButton(ui->aboutButton);
  deselectedPushButton(ui->updateEmpButton);
  deselectedPushButton(ui->addEmpButton);
  // dbUtils.setEmployeeDetails(ui->tableWidget);
  ui->searchStackedWidget->setCurrentIndex(0);
}

/*add employee button*/
void MainWindow::on_addEmpButton_clicked() {
  // emit
  emit sendHeader(msg::infoDemp);

  ui->searchStackedWidget->setCurrentIndex(3);
  // select button
  selectedPushButton(ui->addEmpButton);
  deselectedPushButton(ui->deleteEmpButton);
  deselectedPushButton(ui->techButton);
  deselectedPushButton(ui->aboutButton);
  deselectedPushButton(ui->updateEmpButton);
  deselectedPushButton(ui->searchButton);

  // third stacked widget

  // ui->empDept->setModel(dbUtils.getDepartmentList());

  // ui->empDesig->setModel(dbUtils.getDesignationList(ui->empDept));
}

void MainWindow::on_updateEmpButton_clicked() {
  selectedPushButton(ui->updateEmpButton);
  deselectedPushButton(ui->deleteEmpButton);
  deselectedPushButton(ui->techButton);
  deselectedPushButton(ui->aboutButton);
  deselectedPushButton(ui->searchButton);
  deselectedPushButton(ui->addEmpButton);

  dbUtils.setEmployeeUpdateDetails(ui->updateTableView);
  ui->searchStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButton_5_clicked() {

  if (ui->empName->text().trimmed().isEmpty() ||
      ui->empAddress->text().trimmed().isEmpty() ||
      ui->empEmail->text().trimmed().isEmpty() ||
      ui->empFather->text().trimmed().isEmpty() ||
      ui->empPhone->text().trimmed().isEmpty() ||
      ui->empSalary->text().trimmed().isEmpty() ||
      ui->empID->text().trimmed().isEmpty()) {
    QMessageBox::critical(this, "Empty", "All fields are mandatory to fill");
  } else {

    emit sendHeader(
        msg::header::addEmployee,
        {ui->empID->text().trimmed(), ui->empName->text().trimmed(),
         ui->empDOB->text().trimmed(), ui->empGender->currentText().trimmed(),
         ui->empFather->text().trimmed(), ui->empEmail->text().trimmed(),
         ui->empPhone->text().trimmed(), ui->empAddress->text().trimmed(),
         ui->empDept->currentText().trimmed(),
         ui->empDesig->currentText().trimmed(), ui->empDOJ->text().trimmed(),
         ui->empSalary->text().trimmed(), ui->empType->currentText()});

    //        QSqlQuery *sqlQuery = new QSqlQuery(dbUtils.db);
    //        if (dbUtils.db.isOpen())
    //        {     sqlQuery->prepare(
    //              "insert into Employee (ID, Name, DOB, Gender, Father, Email,
    //              " Phone, " "Address, Department, Designation, DOJ, Salary,
    //              Type) " "values ('" + ui->empID->text().trimmed() + "' ,'" +
    //              ui->empName->text().trimmed() +
    //              "' , '" + ui->empDOB->text().trimmed() + "' , '" +
    //              ui->empGender->currentText().trimmed() + "' , '" +
    //              ui->empFather->text().trimmed() + "' , '" +
    //              ui->empEmail->text().trimmed() + "' , '" +
    //              ui->empPhone->text().trimmed() + "' , '" +
    //              ui->empAddress->text().trimmed() + "' , '" +
    //              ui->empDept->currentText().trimmed() + "' , '" +
    //              ui->empDesig->currentText().trimmed() + "' , '" +
    //              ui->empDOJ->text().trimmed() + "' , " +
    //              ui->empSalary->text().trimmed() + ", '" +
    //              ui->empType->currentText() +
    //              "');");
    //          if (sqlQuery->exec()) {
    //            QMessageBox::information(this, "Success", "Data inserted
    //            successfully");
    //          } else {
    //            qDebug() << "Error : " << sqlQuery->lastError().text()
    //                     << sqlQuery->isValid();
    //          }
    //        }
    //  }
  }
}
/*Generate ID for employee*/
/*Get salary*/
void MainWindow::on_pushButton_2_clicked() {
  // int salary = 0;

  emit sendHeader(msg::id,
                  {ui->empDept->currentText(), ui->empDesig->currentText()});

  //   ui->empID->setText(dbUtils.getEmployeeID(dbUtils.getDepartmentShortName(ui->empDept),
  //   dbUtils.getDesignationShortName(ui->empDesig)));
  /*
      if(ui->empType->currentIndex() == 0)
      {
          salary = dbUtils.getSalary(ui->empDept , ui->empDesig) - 100;
          ui->empSalary->setText(QString::number(salary));
      }
      else
      {
          salary = dbUtils.getSalary(ui->empDept , ui->empDesig);
          ui->empSalary->setText(QString::number(salary));
      }
  */
}

void MainWindow::on_empDept_currentIndexChanged(const QString &arg1) {
  // ui->empDesig->setModel(dbUtils.getDesignationList(ui->empDept));
  // emit sendHeader(msg::header::infoDesig, {ui->empDept->currentText()});
}

void MainWindow::on_deleteEmpButton_clicked() {
  selectedPushButton(ui->deleteEmpButton);
  deselectedPushButton(ui->updateEmpButton);
  deselectedPushButton(ui->techButton);
  deselectedPushButton(ui->aboutButton);
  deselectedPushButton(ui->searchButton);
  deselectedPushButton(ui->addEmpButton);

  // dbUtils.setEmployeeDetails(ui->deleteTableView);
  ui->searchStackedWidget->setCurrentIndex(5);
}

// void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
//{
//     QString id = ui->tableView->model()->index(index.row(),
//     0).data().toString();

//    dbUtils.closeDBConnection();
//    MainWindow::close();
//    EmployeeInfo* emp = new EmployeeInfo(this, id);
//    emp->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
//    emp->show();
//}

void MainWindow::on_pushButton_7_clicked() {
  if (ui->updateEmpName->text().trimmed().isEmpty() ||
      ui->updateEmpAddress->text().trimmed().isEmpty() ||
      ui->updateEmpEmail->text().trimmed().isEmpty() ||
      ui->updateEmpPhone->text().trimmed().isEmpty() ||
      ui->updateEmpFName->text().trimmed().isEmpty()) {
    QMessageBox::warning(this, "Empty Data", "All fields are required to fill");
  } else {
    QSqlQuery *sqlQuery = new QSqlQuery(dbUtils.db);
    if (dbUtils.db.isOpen()) {
      sqlQuery->prepare(
          "UPDATE employee SET "
          "name='" +
          ui->updateEmpName->text().trimmed() + "'," + "father='" +
          ui->updateEmpFName->text().trimmed() + "'," + "email='" +
          ui->updateEmpEmail->text().trimmed() + "'," + "phone='" +
          ui->updateEmpPhone->text().trimmed() + "'," + "address='" +
          ui->updateEmpAddress->text().trimmed() + "'," + "gender='" +
          ui->updateEmpGender->currentText() + "'," + "dob='" +
          ui->updateEmpDOB->text().trimmed() + "' WHERE id='" +
          ui->updateEmpID->text() + "';");

      if (sqlQuery->exec()) {
        QMessageBox::information(this, "Success", "Data updated successfully");
      } else {
        qDebug() << "Error : " << sqlQuery->lastError().text()
                 << sqlQuery->isValid();
      }
    }
  }
}

void MainWindow::on_updateTableView_doubleClicked(const QModelIndex &index) {
  QString id =
      ui->updateTableView->model()->index(index.row(), 0).data().toString();
  ui->updateEmpID->setText(id);

  QSqlQuery *qry = dbUtils.showEmployeeDetailsToLineEdit(id);

  QString dateOfBirth = qry->value("dob").toString();
  QStringList dobSplit = dateOfBirth.split("-");
  QString yearStr = dobSplit[2];
  QString monthStr = dobSplit[1];
  QString dayStr = dobSplit[0];
  QDate date;
  date.setDate(yearStr.toInt(), monthStr.toInt(), dayStr.toInt());

  QString gender = qry->value("gender").toString();

  if (gender == "Male") {
    ui->updateEmpGender->setCurrentIndex(0);
  } else if (gender == "Female") {
    ui->updateEmpGender->setCurrentIndex(1);
  } else {
    ui->updateEmpGender->setCurrentIndex(2);
  }

  ui->updateEmpName->setText(qry->value("name").toString());
  ui->updateEmpFName->setText(qry->value("father").toString());
  ui->updateEmpEmail->setText(qry->value("email").toString());
  ui->updateEmpDOB->setDate(date);
  ui->updateEmpAddress->setText(qry->value("address").toString());
  ui->updateEmpPhone->setText(qry->value("phone").toString());
}

void MainWindow::on_pushButton_8_clicked() {
  ui->updateEmpName->setText("");
  ui->updateEmpFName->setText("");
  ui->updateEmpEmail->setText("");
  ui->updateEmpAddress->setText("");
  ui->updateEmpPhone->setText("");
  ui->updateEmpGender->setCurrentIndex(0);
}

void MainWindow::on_deleteTableView_doubleClicked(const QModelIndex &index) {
  QString id =
      ui->deleteTableView->model()->index(index.row(), 0).data().toString();

  QMessageBox::StandardButton reply;

  reply = QMessageBox::question(
      this, "Confirm", "Are you sure you want to delete '" + id + "' record");

  if (reply == QMessageBox::Yes) {
    dbUtils.deleteEmployeeRecord(id);
  }
}

// void MainWindow::on_searchTextBox_returnPressed()
//{
//     ui->tableView->setModel(NULL);
//     dbUtils.searchEmployeeDetails(ui->tableView,
//     ui->searchTextBox->text().trimmed());
// }

void MainWindow::selectedPushButton(QPushButton *button) {
  button->setStyleSheet(
      "QPushButton{background:#333333;border: none;border-left:6px solid "
      "#1E90FF;margin: 0px;padding: 0px;color:white;text-align: "
      "left;padding-left:24px;}");
}

void MainWindow::deselectedPushButton(QPushButton *button) {
  button->setStyleSheet(
      "QPushButton{ background:#1F1F1F; border: none; margin: 0px; padding: "
      "0px; border-left:6px solid #1F1F1F; color:white; text-align: "
      "left;padding-left:24px;} QPushButton:hover{background:#333333;border: "
      "none;border-left:6px solid #333333;margin: 0px;padding: "
      "0px;color:white;text-align: left;padding-left:24px;}");
}

void MainWindow::on_pushButton_6_clicked() {
  ui->empName->setText("");
  ui->empAddress->setText("");
  ui->empEmail->setText("");
  ui->empFather->setText("");
  ui->empPhone->setText("");
  ui->empSalary->setText("");
  ui->empID->setText("");
}

void MainWindow::on_techButton_clicked() {

  MainWindow::close();
  About *abt = new About(this);
  abt->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  abt->show();
}

// void MainWindow::on_pushButton_3_clicked()
//{
//     dbUtils.setEmployeeDetails(ui->tableWidget);
// }

void MainWindow::receiveSocket(QTcpSocket *socket, quint64 &myuniqueId) {
  qDebug() << "<! CLIENT: FUNCTION RECEIVE SOCKET !>";
  // socket->close();
  // this->socket = new QTcpSocket();
  // this->socket->connectToHost("127.0.0.1", 8080);

  this->socket = socket;

  // this->socket = socket;
  connect(this->socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
  if (this->socket) {
    if (this->socket->isOpen()) {
      this->myUniqueID = myuniqueId;
      emit sendHeader(msg::employeeDetail);
      // emit sendHeader(hd.infoDemp);
    } else

      QMessageBox::critical(this, "QTCPClient",
                            "Socket doesn't seem to be opened");
  }

  else {
    QMessageBox::critical(this, "QTCPClient", "Not connected");
  }
}

void MainWindow::sendHeaderToServer(quint8 header, QVariantList list) {
  if (socket) {
    if (socket->isOpen()) {
      QDataStream socketStream(socket);
      socketStream.setVersion(QDataStream::Qt_5_15);

      // send header
      qDebug() << "To server sended header " << header << " + my id"
               << myUniqueID << '\n';
      socketStream << header << this->myUniqueID;
      if (!list.empty()) {

        for (auto &i : list) {
          qDebug() << "QVariant -> " << i.toByteArray() << "\n";
          // socketStream << i.toByteArray();
          socketStream << i;
        }
      }

    } else {
      QMessageBox::critical(this, "QTCPClient",
                            "Socket doesn't seem to be opened");
    }
  } else {
    QMessageBox::critical(this, "QTCPClient", "Not connected");
  }
}

void MainWindow::sendDempNameForDesign(QString deptname) {
  if (!socket) {
    QMessageBox::critical(this, "QTCPClient", "Not connected");
    return;
  }

  if (!socket->isOpen()) {
    QMessageBox::critical(this, "QTCClient",
                          "Socket doesn't seem to be opened");
    return;
  }

  QDataStream socketStream(socket);
  socketStream.setVersion(QDataStream::Qt_5_15);
  socketStream << deptname;
}

void MainWindow::on_aboutButton_clicked() {
  MainWindow::close();
  TechUsed *tech = new TechUsed(this);
  tech->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  tech->show();
}

void MainWindow::on_empDept_currentTextChanged(const QString &arg1) {
  qDebug() << "From Curent Text Changed " << ui->empDept->currentText();
  // ui->empDesig->setModel(dbUtils.getDesignationList(ui->empDept));
  if (ui->empDept->count() > 0)
    emit sendHeader(msg::header::infoDesig, {ui->empDept->currentText()});
}
