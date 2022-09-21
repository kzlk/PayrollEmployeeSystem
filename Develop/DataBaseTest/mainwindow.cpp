#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString serverName = "localhost\\sqlexpress";
    QString dbName = "LibraryDB";

    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
    db.setConnectOptions();
    QString dsn= QString("DRIVER={SQL Server};Server=%1;Database=%2;").arg(serverName).arg(dbName);
    db.setDatabaseName(dsn);

        if(!db.open())
        {
            QMessageBox *sts = new QMessageBox;
            sts->setText("Connect to SQL Server is faild!");
            sts->exec();
        }
        else
        {
             QMessageBox *sts = new QMessageBox;
             sts->setText("Connect to SQL Server is success!");
             sts->exec();

             QSqlQuery *qry;
             qry = new QSqlQuery;
             QString sQuery ="SELECT book.title, category.category_name FROM category LEFT OUTER JOIN book ON book.category_id = category.id";

             QJsonObject book;

             if(qry->exec(sQuery))
             {
                while(qry->next())
                {
                    qDebug() << qry->value(1).toString();
                    book["title"] = qry->value(0).toString();
                    book["category"] = qry->value(1).toString();

                }
             }else
             {
                qDebug() << "Error in query";
             }

             //qDebug() << book.value("title");

             QJsonDocument doc(book);
             QByteArray bytes = doc.toJson();

             qDebug() << bytes;


             query = new QSqlQuery(db);

             model = new QSqlTableModel(this, db);
             model->setTable("book");
             model->select();
             ui->tableView->setModel(model);

        }



}

MainWindow::~MainWindow()
{
    delete ui;
}

