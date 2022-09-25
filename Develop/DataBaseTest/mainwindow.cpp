#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QSqlRecord>
#include <QAbstractItemModel>
#include <QSqlField>
#include <QTableView>
#include <QTableWidget>
#include <QFile>

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
            db.close();
        }
        else
        {



             QMessageBox *sts = new QMessageBox;
             sts->setText("Connect to SQL Server is success!");
             sts->exec();

             const int newRow = 5;


             QSqlQuery *qry;
             qry = new QSqlQuery;
             QString sQuery ="SELECT book.title, category.category_name FROM category LEFT OUTER JOIN book ON book.category_id = category.id";
             QTableView *receivedmodel;

             //
             QSqlQueryModel myModel{};
             myModel.setQuery(sQuery);
             //for more data in db
             while (myModel.canFetchMore())
                 myModel.fetchMore();


             QSqlTableModel *newModel = new QSqlTableModel(this);


             newModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
             newModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
             newModel->setHeaderData(1, Qt::Horizontal, tr("Salary"));



             //
             QSqlField field{};
             QSqlRecord record{};

             for(int i = 0; i < myModel.rowCount(); ++i)
             {
                 qDebug() << myModel.record(i).value(0).toString() << '\t' << myModel.record(i).value(1).toString();
                   field.setValue(myModel.record(i).value(0).toString());
                  for(int j = 0; j < myModel.columnCount(); ++j)
                  {
                        field.clear();
                        field.setValue(myModel.record(i).value(j).toString());
                        record.insert(j, field);
                  }
                  newModel->insertRecord(i, record);
                  record.clear();
                // receivedmodel->insertRow(receivedmodel->rowCount());
                // receivedmodel->insertRecord(i,  )
                // beginInsertRows(QModelIndex(), )
             }

             QTableView *view = new QTableView;
                 view->setModel(newModel);
                 view->hideColumn(0); // don't show the ID
                 view->show();


                QTableWidget *pWidget = new QTableWidget( myModel.rowCount(), myModel.columnCount() );
                for( int row = 0; row < myModel.rowCount(); row++ )
                {
                    for( int column = 0;  column < myModel.columnCount() ; column++ )
                    {
                        QString sItem =myModel.record(row).value(column).toString();

                        QVariant oVariant(sItem);

                        // allocate the widget item
                        QTableWidgetItem * poItem = new QTableWidgetItem();
                        poItem->setData( Qt::DisplayRole, oVariant );

                        pWidget->setItem( row, column, poItem );
                    }
                }


                pWidget->show();

                QFile file("D:/file.txt");
                if(!file.open(QIODevice::WriteOnly))
                {
                    QMessageBox *sts = new QMessageBox;
                    sts->setText("Can not open file!");
                    sts->exec();
                }

                QDataStream out(&file);


                out.setVersion(QDataStream::Qt_6_3);

                ;


//             if(qry->exec(sQuery))
//             {
//                while(qry->next())
//                {
//                    qDebug() << qry->value(1).toString();
//                }
//             }else
//             {
//                qDebug() << "Error in query";
//             }

           model = new QSqlTableModel(this, db);
             model->setTable("book");
             model->select();

             out << newModel;

             file.flush();
             file.close();



        }

}

MainWindow::~MainWindow()
{
    delete ui;
}
