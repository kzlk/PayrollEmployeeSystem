#include "mainwindow.h"
#include "qstandardpaths.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QSqlQuery>
#include <QStringList>
#include <QTextDocument>
#include <qapplication.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize folder for saving documents
    folderPath = folderSetting.getFolderPath();
    ui->lineEdit_select_folder->setText(this->folderPath);

    // set style
    ui->empDOB->setCalendarPopup(true);
    ui->empDOJ->setCalendarPopup(true);
    ui->updateEmpDOB->setCalendarPopup(true);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setStyleSheet("alternate-background-color: #E4E4E4;"
                                   "background-color:white;"
                                   "selection-background-color:#1492E6;"
                                   "selection-color:white;");

    QObject::connect(this, &MainWindow::sendHeader,
                     &MainWindow::sendHeaderToServer);

    ui->searchStackedWidget->setCurrentWidget(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// read data from server
void MainWindow::readSocket()
{
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
    if (headCommand == msg::unAutorizate)
    {
        QMessageBox::critical(this, "QTCPClient",
                              "You do not do any action because you are "
                              "unautorized user!\n Please log in!");
        headCommand = ~0;
    }

    // header to QString
    QString header(buffer);

    qDebug() << "\nRECEIVED HEADER " << headCommand << '\n';
    //
    if (headCommand == msg::totalInfoEmployee)
    {
        quint8 headerInfoEmp{};
        qint32 lastID{};
        QByteArray dept{};
        QByteArray design{};

        socketStream >> lastID >> dept >> design >> headerInfoEmp;
        qDebug() << "Data received --> " << headerInfoEmp << '\t' << lastID
                 << '\t' << dept << '\t' << design;
        //
        loadTotalData(lastID, QString(dept), QString(design));

        if (headerInfoEmp == msg::employeeDetail)
        {
            appanedDataToMainTable(socketStream);
        }
        else
        {
            QMessageBox::information(
                this, "QTCPCLIENT",
                QString("header != %1 !").arg(msg::employeeDetail));
        }
    }
    else if (headCommand == msg::infoDemp)
    {
        ui->empDesig->clear();
        ui->searchStackedWidget->setCurrentIndex(3);
        QVector<QString> receiveDep{};
        buffer.clear();
        QByteArray size;
        qint32 rec{};
        size.clear();
        socketStream >> rec;
        qDebug() << "Count Row In " << headCommand << rec << '\n';
        if (rec > 0)
        {
            for (int i = 0; i < rec; ++i)
            {
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
        }
    }
    else if (headCommand == msg::infoDesig)
    {

        QVector<QString> receiveDesig{};
        buffer.clear();
        qint32 rec{};
        socketStream >> rec;
        qDebug() << "Count Row In " << headCommand << rec << '\n';
        if (rec > 0)
        {
            for (int i = 0; i < rec; ++i)
            {
                buffer.clear();
                socketStream >> buffer;
                receiveDesig.emplaceBack(QString(buffer));
                qDebug() << " Row In " + header << receiveDesig.at(i) << '\n';
            }
            ui->empDesig->clear();
            ui->empDesig->addItems(receiveDesig);
            //
        }
    }
    else if (headCommand == msg::id)
    {
        QString id;
        socketStream >> id;
        qDebug() << "Received employee ID  ->   " << id;
        ui->empID->setText(id);

        emit sendHeader(msg::salary, {ui->empDept->currentText(),
                                      ui->empDesig->currentText()});
    }
    else if (headCommand == msg::salary)
    {
        int salary{};
        socketStream >> salary;
        if (ui->empType->currentIndex() == 0)
        {
            salary -= 5;
            ui->empSalary->setText(QString::number(salary));
        }
        else
        {
            ui->empSalary->setText(QString::number(salary));
        }
    }
    else if (headCommand == msg::addEmployee)
    {
        QString status{};
        socketStream >> status;

        if (status == st.success)
        {
            QMessageBox::information(this, "Success",
                                     "Data inserted successfully");
        }
        else if (status == st.failure)
        {
            QMessageBox::critical(this, "Failure",
                                  "Something went wrong when data inserted");
        }
    }
    else if (headCommand == msg::header::insertedInDBSettingStatus)
    {
        QString status{};

        socketStream >> status;

        if (status == st.success)
        {
            QMessageBox::information(this, "Success",
                                     "Data inserted successfully");
        }
        else
        {
            QMessageBox::critical(this, "Failure",
                                  "Something went wrong when data inserted");
        };
    }
    else if (headCommand == msg::header::getSettingsData)
    {
        QString status{};
        socketStream >> status;
        if (status != st.empty)
        {
            QVariantList list{};
            list.reserve(3);

            QVariant start{};
            QVariant end{};
            QVariant next{};
            QVariant freq{};
            bool autoPilot{};
            socketStream >> start >> end >> next >> freq >> autoPilot;
            qDebug() << start << '\n'
                     << end << '\n'
                     << next << '\n'
                     << freq << '\n'
                     << autoPilot;
            setDataInSettingWindow(start, end, next, freq, autoPilot);
        }
        else
        {
            QMessageBox::information(this, "Info", "No data in db found");
            setDataInSettingWindow();
        }
    }
    else if (headCommand == msg::header::getReportPeriodInfo)
    {
        appendDataToReportPagePeriod(socketStream);
    }
    else if (headCommand == msg::header::getDetailInfoPaymentPeriod)
    {
        appendDataToDetailReportPage(socketStream);
    }
    else if (headCommand == msg::header::getPdfData)
    {
        QByteArray status{};
        QString html{};
        QByteArray html1{};
        QByteArray html2{};
        socketStream >> html >> status;
        if (st.success == html)
        {
            auto rs = QString::fromUtf8(status);
            savePdf(rs);
        }
        else
        {
            QMessageBox::information(this, "Info", "Could not generate pdf");
        }
    }
    else if (headCommand == msg::header::getSearchedEmployee)
    {
        appanedDataToMainTable(socketStream);
    }
    else if (headCommand == msg::header::getEmployeeInfo)
    {
        QString status{};
        QVariantList empList{};
        socketStream >> status >> empList;
        if (st.success == status)
        {
            appendDataToEmploeeLabel(empList);
        }
        else
        {
            QMessageBox::information(this, "Info",
                                     "Could not load employee info");
        }
    }
    else if (headCommand == msg::header::deleteEmployee)
    {
        appendDataToDeleteTable(socketStream);
    }
    else if (headCommand == msg::header::deleteInfoEmp)
    {
        QString status{};
        socketStream >> status;

        if (status == st.success)
        {
            QMessageBox::information(this, "Info",
                                     "Employee has successfully deleted");
        }
        else
        {
            QMessageBox::critical(this, "Info", "Could not delete employee");
        }
    }
    else if (headCommand == msg::header::updateEployee)
    {
        appendDataToUpdatePage(socketStream);
    }
    else if (headCommand == msg::header::getOneEmpForUpdate)
    {
        QString status{};
        QVariantList empList{};
        socketStream >> status >> empList;
        if (st.success == status)
        {
            appendDataToLineEditUpdate(empList);
        }
        else
        {
            QMessageBox::information(this, "Info",
                                     "Could not load employee info");
        }
    }
    else if (headCommand == msg::setUpdateOneEmployee)
    {
        QString status{};

        socketStream >> status;
        if (st.success == status)
        {
            QMessageBox::information(this, "Info", "Data successfully updated");
        }
        else
        {
            QMessageBox::information(this, "Info",
                                     "Could not load employee info");
        }
    }
    else
    {
        QMessageBox::information(this, "QTCPCLIENT",
                                 "header != employeeDetail!");
    }

    if (!socketStream.commitTransaction())
    {
        QString message =
            QString("%1 :: Waiting for more data to come..after all condition")
                .arg(socket->socketDescriptor());
        QMessageBox::information(this, "QTCPCLIENT", message);
        QByteArray buff;
        socketStream >> buff;

        qDebug() << ("Comming data: --> " + QString(buffer));
        qDebug() << buff;
        return;
    }
}

// set big three label on main window
void MainWindow::loadTotalData(int lastID, QString dept, QString designTtl)
{
    ui->searchStackedWidget->setCurrentIndex(0);
    QString lastIDString = QString::number(lastID);

    if (lastIDString.length() == 1)
    {
        lastIDString = "00" + lastIDString;
    }
    else if (lastIDString.length() == 2)
    {
        lastIDString = "0" + lastIDString;
    }

    ui->empLabel->setText(lastIDString);

    if (dept.length() == 1)
    {
        dept = "00" + dept;
    }
    else if (dept.length() == 2)
    {
        dept = "0" + dept;
    }

    ui->deptLabel->setText(dept);

    if (designTtl.length() == 1)
    {
        designTtl = "00" + designTtl;
    }
    else if (designTtl.length() == 2)
    {
        designTtl = "0" + designTtl;
    }

    ui->designLabel->setText(designTtl);
}

// SET MOUSE EVENT
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isMouseDown = true;
        mousePoint = event->globalPos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseDown = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint delta = event->globalPos() - mousePoint;
    if (isMouseDown == true)
        move(x() + delta.x(), y() + delta.y());
    else
        move(x() + delta.x(), y() + delta.y());
    mousePoint = event->globalPos();
}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

void MainWindow::on_minimizeButton_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void MainWindow::on_pushButton_clicked()
{
    ui->searchTextBox->setText("");
    emit sendHeader(msg::header::getSearchedEmployee,
                    {ui->searchTextBox->text().trimmed()});
}

/*search button*/
void MainWindow::on_searchButton_clicked()
{
    // send header to get employee info
    emit sendHeader(msg::employeeDetail);
    selectedPushButton(ui->searchButton);
    deselectedPushButton(ui->deleteEmpButton);
    deselectedPushButton(ui->reportButton);
    deselectedPushButton(ui->aboutButton);
    deselectedPushButton(ui->updateEmpButton);
    deselectedPushButton(ui->addEmpButton);
    deselectedPushButton(ui->settingsButton);
    ui->searchStackedWidget->setCurrentIndex(0);
}

/*add employee button*/
void MainWindow::on_addEmpButton_clicked()
{
    // emit
    emit sendHeader(msg::infoDemp);

    ui->searchStackedWidget->setCurrentIndex(3);

    // select button
    selectedPushButton(ui->addEmpButton);
    deselectedPushButton(ui->deleteEmpButton);
    deselectedPushButton(ui->reportButton);
    deselectedPushButton(ui->aboutButton);
    deselectedPushButton(ui->updateEmpButton);
    deselectedPushButton(ui->searchButton);
    deselectedPushButton(ui->settingsButton);
}

void MainWindow::on_updateEmpButton_clicked()
{
    selectedPushButton(ui->updateEmpButton);
    deselectedPushButton(ui->deleteEmpButton);
    deselectedPushButton(ui->reportButton);
    deselectedPushButton(ui->aboutButton);
    deselectedPushButton(ui->searchButton);
    deselectedPushButton(ui->addEmpButton);
    deselectedPushButton(ui->settingsButton);

    emit sendHeader(msg::header::updateEployee);
    ui->searchStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButton_5_clicked()
{

    if (ui->empName->text().trimmed().isEmpty() ||
        ui->empAddress->text().trimmed().isEmpty() ||
        ui->empEmail->text().trimmed().isEmpty() ||
        ui->empFather->text().trimmed().isEmpty() ||
        ui->empPhone->text().trimmed().isEmpty() ||
        ui->empSalary->text().trimmed().isEmpty() ||
        ui->empID->text().trimmed().isEmpty())
    {
        QMessageBox::critical(this, "Empty",
                              "All fields are mandatory to fill");
    }
    else
    {
        emit sendHeader(
            msg::header::addEmployee,
            {ui->empID->text().trimmed(), ui->empName->text().trimmed(),
             ui->empDOB->text().trimmed(),
             ui->empGender->currentText().trimmed(),
             ui->empFather->text().trimmed(), ui->empEmail->text().trimmed(),
             ui->empPhone->text().trimmed(), ui->empAddress->text().trimmed(),
             ui->empDept->currentText().trimmed(),
             ui->empDesig->currentText().trimmed(),
             ui->empDOJ->text().trimmed(), ui->empSalary->text().trimmed(),
             ui->empType->currentText()});
    }
}

/*Generate ID for employee*/
/*Get salary*/
void MainWindow::on_pushButton_2_clicked()
{

    emit sendHeader(msg::id,
                    {ui->empDept->currentText(), ui->empDesig->currentText()});
}

void MainWindow::on_deleteEmpButton_clicked()
{
    selectedPushButton(ui->deleteEmpButton);
    deselectedPushButton(ui->updateEmpButton);
    deselectedPushButton(ui->reportButton);
    deselectedPushButton(ui->aboutButton);
    deselectedPushButton(ui->searchButton);
    deselectedPushButton(ui->addEmpButton);
    deselectedPushButton(ui->settingsButton);

    emit sendHeader(msg::header::deleteEmployee);

    ui->searchStackedWidget->setCurrentIndex(5);
}

void MainWindow::on_pushButton_7_clicked()
{
    if (ui->updateEmpName->text().trimmed().isEmpty() ||
        ui->updateEmpAddress->text().trimmed().isEmpty() ||
        ui->updateEmpEmail->text().trimmed().isEmpty() ||
        ui->updateEmpPhone->text().trimmed().isEmpty() ||
        ui->updateEmpFName->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Empty Data",
                             "All fields are required to fill");
    }
    else
    {
        emit sendHeader(msg::header::setUpdateOneEmployee,
                        {ui->updateEmpName->text().trimmed(),
                         ui->updateEmpFName->text().trimmed(),
                         ui->updateEmpEmail->text().trimmed(),
                         ui->updateEmpPhone->text().trimmed(),
                         ui->updateEmpAddress->text().trimmed(),
                         ui->updateEmpGender->currentText(),
                         ui->updateEmpDOB->text().trimmed(),
                         ui->updateEmpID->text()});
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->updateEmpName->setText("");
    ui->updateEmpFName->setText("");
    ui->updateEmpEmail->setText("");
    ui->updateEmpAddress->setText("");
    ui->updateEmpPhone->setText("");
    ui->updateEmpGender->setCurrentIndex(0);
}

/*Search emploee*/
void MainWindow::on_searchTextBox_returnPressed()
{
    ui->tableWidget->clear();

    emit sendHeader(msg::header::getSearchedEmployee,
                    {ui->searchTextBox->text().trimmed()});
}

void MainWindow::selectedPushButton(QPushButton *button)
{
    button->setStyleSheet(
        "QPushButton{background:#333333;border: none;border-left:6px solid "
        "#44FF17;margin: 0px;padding: 0px;color:white;text-align: "
        "left;padding-left:24px;}");
}

void MainWindow::deselectedPushButton(QPushButton *button)
{
    button->setStyleSheet(
        "QPushButton{ background:#1F1F1F; border: none; margin: 0px; padding: "
        "0px; border-left:6px solid #1F1F1F; color:white; text-align: "
        "left;padding-left:24px;} QPushButton:hover{background:#333333;border: "
        "none;border-left:6px solid #333333;margin: 0px;padding: "
        "0px;color:white;text-align: left;padding-left:24px;}");
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->empName->setText("");
    ui->empAddress->setText("");
    ui->empEmail->setText("");
    ui->empFather->setText("");
    ui->empPhone->setText("");
    ui->empSalary->setText("");
    ui->empID->setText("");
}

void MainWindow::on_techButton_clicked()
{
    deselectedPushButton(ui->deleteEmpButton);
    deselectedPushButton(ui->aboutButton);
    deselectedPushButton(ui->updateEmpButton);
    deselectedPushButton(ui->addEmpButton);
    deselectedPushButton(ui->searchButton);
    deselectedPushButton(ui->settingsButton);
    deselectedPushButton(ui->reportButton);
    ui->searchStackedWidget->setCurrentWidget(ui->page_report);
}

void MainWindow::receiveSocket(QTcpSocket *socket, quint64 &myuniqueId)
{
    qDebug() << "<! CLIENT: FUNCTION RECEIVE SOCKET !>";

    this->socket = socket;

    // this->socket = socket;
    connect(this->socket, &QTcpSocket::readyRead, this,
            &MainWindow::readSocket);
    if (this->socket)
    {
        if (this->socket->isOpen())
        {
            this->myUniqueID = myuniqueId;
            emit sendHeader(msg::employeeDetail);
            // emit sendHeader(hd.infoDemp);
        }
        else

            QMessageBox::critical(this, "QTCPClient",
                                  "Socket doesn't seem to be opened");
    }

    else
    {
        QMessageBox::critical(this, "QTCPClient", "Not connected");
    }
}

void MainWindow::sendHeaderToServer(quint8 header, QVariantList list)
{
    if (socket)
    {
        if (socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            // send header
            qDebug() << "To server sended header " << header << " + my id"
                     << myUniqueID << '\n';
            socketStream << header << this->myUniqueID;
            if (!list.empty())
            {

                for (auto &i : list)
                {
                    qDebug() << "QVariant -> " << i.toByteArray() << "\n";
                    socketStream << i;
                }
            }
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

void MainWindow::sendDempNameForDesign(QString deptname)
{
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPClient", "Not connected");
        return;
    }

    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCClient",
                              "Socket doesn't seem to be opened");
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);
    socketStream << deptname;
}

void MainWindow::appendDataToReportPagePeriod(QDataStream &socketStream)
{
    quint32 row{};
    quint32 column{};
    /*Receive row and column*/
    socketStream >> row >> column;
    qDebug() << " appendDataToReportPagePeriod \nRow received = " << row
             << " | Column received = " << column;
    ui->tableWidget_payment_period->horizontalHeader()->setVisible(false);
    ui->tableWidget_payment_period->verticalHeader()->setVisible(false);
    ui->tableWidget_payment_period->setShowGrid(false);
    ui->tableWidget_payment_period->setStyleSheet(
        "QTableView {selection-background-color: #44FF17;}");
    ui->tableWidget_payment_period->setEditTriggers(
        QAbstractItemView::NoEditTriggers);
    ui->tableWidget_payment_period->setSelectionBehavior(
        QAbstractItemView::SelectRows);
    ui->tableWidget_payment_period->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->tableWidget_payment_period->setRowCount(row);
    ui->tableWidget_payment_period->setColumnCount(column);
    ui->tableWidget_payment_period->setWordWrap(true);

    ui->tableWidget_payment_period->setColumnWidth(0, ui->label_36->width());
    ui->tableWidget_payment_period->setColumnWidth(1, ui->label_47->width());
    ui->tableWidget_payment_period->setColumnWidth(2, ui->label_37->width());
    ui->tableWidget_payment_period->setColumnWidth(3, ui->label_45->width());
    ui->tableWidget_payment_period->setColumnWidth(4, ui->label_38->width());
    ui->tableWidget_payment_period->setColumnWidth(4, ui->label_46->width());

    /*Receive table data*/
    if (row > 0 && column > 0)
    {
        for (int r = 0; r < row; ++r)
        {
            for (int col = 0; col < column; ++col)
            {
                QVariant receivedItem{};
                socketStream >> receivedItem;
                qDebug() << receivedItem;
                QTableWidgetItem *poItem = new QTableWidgetItem();
                poItem->setData(Qt::DisplayRole, receivedItem);
                ui->tableWidget_payment_period->setItem(r, col, poItem);
            }
        }
    }
    else
    {
        // TODO: Error handling
        QMessageBox::information(
            this, "QTCPCLIENT", QString("There are no payments record found!"));
    }
}

void MainWindow::appendDataToDetailReportPage(QDataStream &socketStream)
{
    quint32 row{};
    quint32 column{};
    /*Receive row and column*/
    socketStream >> row >> column;

    qDebug() << " appendDataToReportPagePeriod \nRow received = " << row
             << " | Column received = " << column;

    ui->tableWidget_payment_detail->horizontalHeader()->setVisible(false);
    ui->tableWidget_payment_detail->verticalHeader()->setVisible(false);
    ui->tableWidget_payment_detail->setShowGrid(false);
    ui->tableWidget_payment_detail->setStyleSheet(
        "QTableView {selection-background-color: #44FF17;}");
    ui->tableWidget_payment_detail->setEditTriggers(
        QAbstractItemView::NoEditTriggers);
    ui->tableWidget_payment_detail->setSelectionBehavior(
        QAbstractItemView::SelectRows);
    ui->tableWidget_payment_detail->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->tableWidget_payment_detail->setRowCount(row);
    ui->tableWidget_payment_detail->setColumnCount(column);
    ui->tableWidget_payment_detail->setWordWrap(true);

    ui->tableWidget_payment_detail->setColumnWidth(0, 90);
    ui->tableWidget_payment_detail->setColumnWidth(1,
                                                   ui->label_11->width() - 90);
    ui->tableWidget_payment_detail->setColumnWidth(2, ui->label_42->width());
    ui->tableWidget_payment_detail->setColumnWidth(3, ui->label_41->width());
    ui->tableWidget_payment_detail->setColumnWidth(4, ui->label_44->width());
    ui->tableWidget_payment_detail->setColumnWidth(5, ui->label_40->width());
    this->idEmp.clear();

    /*Receive table data*/
    if (row > 0 && column > 0)
    {
        for (int r = 0; r < row; ++r)
        {
            for (int col = 0; col < column; ++col)
            {
                QVariant receivedItem{};
                socketStream >> receivedItem;
                qDebug() << receivedItem;
                QTableWidgetItem *poItem = new QTableWidgetItem();
                poItem->setData(Qt::DisplayRole, receivedItem);
                ui->tableWidget_payment_detail->setItem(r, col, poItem);
            }
        }
    }
    else
    {
        QMessageBox::information(
            this, "QTCPCLIENT", QString("There are no payments record found!"));
    }
}

void MainWindow::appendDataToEmploeeLabel(QVariantList list)
{
    // Personal Info
    ui->showEmpName->setText(list.at(0).toString());
    ui->showEmpDOB->setText(list.at(1).toString());
    ui->showEmpFather->setText(list.at(3).toString());
    ui->showEmpEmail->setText(list.at(4).toString());
    ui->showEmpPhone->setText(list.at(5).toString());
    ui->showEmpAddress->setText(list.at(6).toString());

    // Company Info
    ui->dept->setText(list.at(7).toString());
    ui->design->setText(list.at(8).toString());
    ui->doj->setText(list.at(9).toString());
    ui->salary->setText(list.at(10).toString());
    ui->type->setText(list.at(11).toString());
}

void MainWindow::setDataInSettingWindow(QVariant start, QVariant end,
                                        QVariant next, QVariant freq,
                                        bool autopilot)
{
    if (!start.isValid() || !end.isValid() || !next.isValid() ||
        !freq.isValid())
    {
        ui->dateTimeEdit->clear();
        ui->label_upcom_pay->setText("Unknown");
        ui->comboBox_autopilot->setCurrentIndex(0);
    }
    else
    {
        QMap<quint8, quint8> comboBox{{2, 0}, {30, 1}, {7, 2}};
        auto search = comboBox.find(freq.toInt());
        ui->dateTimeEdit->setDateTime(end.toDateTime());
        ui->label_upcom_pay->setText(next.toDateTime().toString());
        ui->comboBox_Frequency->setCurrentIndex(search.value());
        ui->comboBox_autopilot->setCurrentIndex(int(autopilot));
    }
}

void MainWindow::appendDataToLineEditUpdate(QVariantList list)
{
    ui->updateEmpName->setText(list.at(0).toString());
    ui->updateEmpFName->setText(list.at(1).toString());

    if (list.at(2).toString() == "Male")
    {
        ui->updateEmpGender->setCurrentIndex(0);
    }
    else if (list.at(2).toString() == "Female")
    {
        ui->updateEmpGender->setCurrentIndex(1);
    }

    ui->updateEmpDOB->setDate(list.at(3).toDate());
    ui->updateEmpPhone->setText(list.at(4).toString());
    ui->updateEmpEmail->setText(list.at(5).toString());
    ui->updateEmpAddress->setText(list.at(6).toString());
}

bool MainWindow::checkFolder(QString &path)
{
    QFileInfo folder(path);
    if (!folder.exists())
    {
        QMessageBox::warning(this, "Error! ", "Folder doesn't exists");
        return false;
    }

    if (!folder.isDir())
    {
        QMessageBox::warning(this, "Error! ", "This is not a directory!");
        return false;
    }

    return true;
}

void MainWindow::savePdf(QString &html)
{
    QTextDocument document;
    document.setHtml(html);
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);
    // auto a = QFile::exists( folderPath +
    // ui->tableWidget_payment_detail->selectedItems()[0]->text());
    auto fileName =
        ui->tableWidget_payment_detail->selectedItems()[0]->text().replace("/",
                                                                           "_");
    printer.setOutputFileName(folderPath + fileName + ".pdf");

    printer.setPageMargins(QMarginsF(0, 0, 0, 0));
    document.print(&printer);

    QMessageBox::information(
        this, "QTCPCLIENT",
        QString("File saved! to " + folderPath + fileName + ".pdf"));
}

void MainWindow::on_empDept_currentTextChanged(const QString &arg1)
{
    qDebug() << "From Curent Text Changed " << ui->empDept->currentText();
    // ui->empDesig->setModel(dbUtils.getDesignationList(ui->empDept));
    if (ui->empDept->count() > 0)
        emit sendHeader(msg::header::infoDesig, {ui->empDept->currentText()});
}

void MainWindow::on_button_applyAutoPilot_clicked()
{
    /*Check socket*/
    if (!socket)
    {
        QMessageBox::critical(this, "QTCPClient", "Not connected");
        return;
    }
    /*Check is socket open*/
    if (!socket->isOpen())
    {
        QMessageBox::critical(this, "QTCClient",
                              "Socket doesn't seem to be opened");
        return;
    }

    /*Calculate start payment date*/
    QDateTime startPeriod = calculateStartPayPeriod(
        ui->dateTimeEdit->dateTime(), ui->comboBox_Frequency->currentText());

    /*Calculate next payment date*/
    QDateTime nextPaymentPeriod = calculateNextPaymentPeriod(
        ui->dateTimeEdit->dateTime(), ui->comboBox_Frequency->currentText());

    /*Set next period to label*/
    ui->label_upcom_pay->setText(nextPaymentPeriod.toString());

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    /*Send data to server*/
    emit sendHeader(
        msg::header::insertInDBSettingPayment,
        {startPeriod, ui->dateTimeEdit->dateTime(), nextPaymentPeriod,
         converAutoPilotToInteger(ui->comboBox_autopilot->currentText()),
         converFrequencyToInteger(ui->comboBox_Frequency->currentText())});
}

QDateTime MainWindow::calculateStartPayPeriod(QDateTime datetime, QString box)
{
    QDateTime startPeriod{};

    if (box == "Montly")
    {
        startPeriod = datetime.addDays(-30);
    }
    else if (box == "EveryWeek")
    {
        startPeriod = datetime.addDays(-7);
    }
    else if (box == "EveryTwoDays")
    {
        startPeriod = datetime.addDays(-2);
    }
    else
    {
        QMessageBox::critical(this, "QTCPClient", "Unknown payment frequency");
    }
    return startPeriod;
}

QDateTime MainWindow::calculateNextPaymentPeriod(QDateTime datetime,
                                                 QString box)
{
    QDateTime startPeriod{};

    if (box == "Montly")
    {
        startPeriod = datetime.addDays(30);
    }
    else if (box == "EveryWeek")
    {
        startPeriod = datetime.addDays(7);
    }
    else if (box == "EveryTwoDays")
    {
        startPeriod = datetime.addDays(2);
    }
    else
    {
        QMessageBox::critical(this, "QTCPClient", "Unknown payment frequency");
    }
    return startPeriod;
}

qint8 MainWindow::converAutoPilotToInteger(QString box)
{
    qint8 res{};
    if (box == "On")
    {
        return 1;
    }
    else if (box == "Off")
    {
        return 0;
    }
    else
    {
        QMessageBox::critical(this, "QTCPClient", "Unknown payment frequency");
        return ~0;
    }
    return res;
}

qint8 MainWindow::converFrequencyToInteger(QString box)
{
    qint8 res{};
    if (box == "Montly")
    {
        res = (30);
    }
    else if (box == "EveryWeek")
    {
        res = (7);
    }
    else if (box == "EveryTwoDays")
    {
        res = (2);
    }
    else
    {
        QMessageBox::critical(this, "QTCPClient", "Unknown payment frequency");
    }
    return res;
}

void MainWindow::on_settingsButton_clicked()
{
    deselectedPushButton(ui->deleteEmpButton);
    deselectedPushButton(ui->reportButton);
    deselectedPushButton(ui->aboutButton);
    deselectedPushButton(ui->updateEmpButton);
    deselectedPushButton(ui->addEmpButton);
    deselectedPushButton(ui->searchButton);
    selectedPushButton(ui->settingsButton);

    ui->searchStackedWidget->setCurrentWidget(ui->settingWidget);

    emit sendHeader(msg::header::getSettingsData);
}

void MainWindow::appanedDataToMainTable(QDataStream &socketStream)
{
    ui->searchStackedWidget->setCurrentWidget(0);
    quint32 row{};
    quint32 column{};
    /*Receive row and column*/
    socketStream >> row >> column;
    qDebug() << "\nRow received = " << row << " | Column received = " << column;
    ui->tableWidget->horizontalHeader()->setVisible(false);
    // ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setStyleSheet(
        "QTableView {selection-background-color: #44FF17;}");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidget->setRowCount(row);
    ui->tableWidget->setColumnCount(column);
    ui->tableWidget->setWordWrap(true);
    ui->tableWidget->setColumnWidth(0, ui->label_8->width());
    ui->tableWidget->setColumnWidth(1, ui->label_21->width());
    ui->tableWidget->setColumnWidth(2, ui->label_22->width());
    ui->tableWidget->setColumnWidth(3, ui->label_23->width());
    ui->tableWidget->setColumnWidth(4, ui->label_24->width());
    ui->tableWidget->setColumnWidth(5, ui->label_25->width());

    /*Receive table data*/
    if (row > 0 && column > 0)
    {
        for (int r = 0; r < row; ++r)
        {
            for (int col = 0; col < column; ++col)
            {

                QVariant receivedItem{};
                socketStream >> receivedItem;
                qDebug() << receivedItem;
                QTableWidgetItem *poItem = new QTableWidgetItem();
                poItem->setData(Qt::DisplayRole, receivedItem);
                ui->tableWidget->setItem(r, col, poItem);
            }
        }
    }
}

void MainWindow::appendDataToDeleteTable(QDataStream &socketStream)
{
    // ui->searchStackedWidget->setCurrentWidget(0);

    quint32 row{};
    quint32 column{};
    /*Receive row and column*/
    socketStream >> row >> column;
    qDebug() << "\nRow received = " << row << " | Column received = " << column;
    ui->tableWidget_delete->horizontalHeader()->setVisible(false);
    // ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget_delete->setShowGrid(false);
    ui->tableWidget_delete->setStyleSheet(
        "QTableView {selection-background-color: #44FF17;}");
    ui->tableWidget_delete->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_delete->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_delete->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->tableWidget_delete->setRowCount(row);
    ui->tableWidget_delete->setColumnCount(column);
    ui->tableWidget_delete->setWordWrap(true);
    ui->tableWidget_delete->setColumnWidth(0, ui->label_8->width());
    ui->tableWidget_delete->setColumnWidth(1, ui->label_21->width());
    ui->tableWidget_delete->setColumnWidth(2, ui->label_22->width());
    ui->tableWidget_delete->setColumnWidth(3, ui->label_23->width());
    ui->tableWidget_delete->setColumnWidth(4, ui->label_24->width());
    ui->tableWidget_delete->setColumnWidth(5, ui->label_25->width());

    /*Receive table data*/
    if (row > 0 && column > 0)
    {
        for (int r = 0; r < row; ++r)
        {
            for (int col = 0; col < column; ++col)
            {

                QVariant receivedItem{};
                socketStream >> receivedItem;
                qDebug() << receivedItem;
                QTableWidgetItem *poItem = new QTableWidgetItem();
                poItem->setData(Qt::DisplayRole, receivedItem);
                ui->tableWidget_delete->setItem(r, col, poItem);
            }
        }
    }
}

void MainWindow::appendDataToUpdatePage(QDataStream &socketStream)
{

    quint32 row{};
    quint32 column{};
    /*Receive row and column*/
    socketStream >> row >> column;
    qDebug() << "\nRow received = " << row << " | Column received = " << column;
    ui->tableWidget_update->horizontalHeader()->setVisible(false);
    // ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget_update->setShowGrid(false);
    ui->tableWidget_update->setStyleSheet(
        "QTableView {selection-background-color: #44FF17;}");
    ui->tableWidget_update->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_update->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_update->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->tableWidget_update->setRowCount(row);
    ui->tableWidget_update->setColumnCount(column);
    ui->tableWidget_update->setWordWrap(true);
    ui->tableWidget_update->setColumnWidth(0, ui->label_26->width());
    ui->tableWidget_update->setColumnWidth(1, ui->label_27->width());
    ui->tableWidget_update->setColumnWidth(2, ui->label_28->width());

    /*Receive table data*/
    if (row > 0 && column > 0)
    {
        for (int r = 0; r < row; ++r)
        {
            for (int col = 0; col < column; ++col)
            {

                QVariant receivedItem{};
                socketStream >> receivedItem;
                qDebug() << receivedItem;
                QTableWidgetItem *poItem = new QTableWidgetItem();
                poItem->setData(Qt::DisplayRole, receivedItem);
                ui->tableWidget_update->setItem(r, col, poItem);
            }
        }
    }
}

void MainWindow::on_reportButton_clicked()
{
    ui->searchStackedWidget->setCurrentWidget(ui->page_report);
    deselectedPushButton(ui->deleteEmpButton);
    selectedPushButton(ui->reportButton);
    deselectedPushButton(ui->aboutButton);
    deselectedPushButton(ui->updateEmpButton);
    deselectedPushButton(ui->addEmpButton);
    deselectedPushButton(ui->searchButton);
    deselectedPushButton(ui->settingsButton);

    emit sendHeader(msg::header::getReportPeriodInfo);
}

/*Get detail payment description*/
void MainWindow::on_tableWidget_payment_period_doubleClicked(
    const QModelIndex &index)
{
    auto id = ui->tableWidget_payment_period->model()
                  ->index(index.row(), 0)
                  .data()
                  .toInt();

    emit sendHeader(msg::header::getDetailInfoPaymentPeriod, {id});

    ui->searchStackedWidget->setCurrentWidget(ui->page_info_payment);
}

void MainWindow::on_biutton_back_period_clicked()
{
    emit sendHeader(msg::header::getReportPeriodInfo);
    ui->searchStackedWidget->setCurrentWidget(ui->page_report);
}

void MainWindow::on_biutton_save_pdf_clicked()
{
    if (!this->checkFolder(folderPath))
    {
        QMessageBox::information(this, "QTCPClient", "Incorrect folder path");
        return;
    }
    // get employee id
    auto i = ui->tableWidget_payment_detail->selectedItems()[0];
    QMessageBox::information(this, "QTCPClient",
                             "Item clicked on " + i->text());

    // send to server command getPdf and employee id
    emit sendHeader(msg::header::getPdfData, {i->text()});
}

void MainWindow::on_btn_changeFolder_clicked()
{
    QString path = QFileDialog::getExistingDirectory(
        this, "Folder choosing",
        QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
    path += "/";
    if (!checkFolder(path))
        return;
    ui->lineEdit_select_folder->setText(path);
    folderPath = path;
    folderSetting.writeFolderPath(path);
}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    QString id =
        ui->tableWidget->model()->index(index.row(), 0).data().toString();

    emit sendHeader(msg::header::getEmployeeInfo, {id});

    ui->searchStackedWidget->setCurrentWidget(ui->page_info_emp);
}

void MainWindow::on_button_back_info_emp_clicked()
{
    ui->searchStackedWidget->setCurrentWidget(ui->searchWidget);
}

void MainWindow::on_tableWidget_delete_doubleClicked(const QModelIndex &index)
{
    QString id = ui->tableWidget_delete->model()
                     ->index(index.row(), 0)
                     .data()
                     .toString();

    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(
        this, "Confirm", "Are you sure you want to delete '" + id + "' record");

    if (reply == QMessageBox::Yes)
    {
        emit sendHeader(msg::header::deleteInfoEmp, {id});
    }
}

void MainWindow::on_button_refresh_clicked()
{
    emit sendHeader(msg::header::deleteEmployee);
}

void MainWindow::on_tableWidget_update_clicked(const QModelIndex &index)
{
    QString id = ui->tableWidget_update->model()
                     ->index(index.row(), 0)
                     .data()
                     .toString();
    ui->updateEmpID->setText(id);

    emit sendHeader(msg::getOneEmpForUpdate, {id});
}
