#include <QtSql/QSqlDatabase>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QLabel>

class EmployeeDatabaseUtils
{
    public:

    QSqlDatabase empDB;

    bool connectToDB(QString dbName)
    {
        empDB = QSqlDatabase::addDatabase("QMYSQL");
        empDB.setDatabaseName(dbName);
        empDB.setHostName("localhost");
        empDB.setUserName("root");
        empDB.setPassword("");

        if(empDB.open())
        {
            qDebug() << ("Connected to employee database");
            return true;
        }
        else
        {
            qDebug() << ("Failed to open the employee database");
            return false;
        }
    }

    void closeDBConnection()
    {

        if(empDB.isValid())
        {
            empDB.close();
            empDB.removeDatabase(QSqlDatabase::defaultConnection);
        }
    }

};
