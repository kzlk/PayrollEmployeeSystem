#include <QtSql/QSqlDatabase>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QLabel>
#include <QTableView>
#include <QSqlDatabase>

class DatabaseUtils
{
    public:

    QString serverName = "localhost\\sqlexpress";
    QString dbName = "payrolldb";
    QSqlDatabase db{};

    bool connectToDB()
    {
       db = QSqlDatabase::addDatabase("QODBC");
       db.setConnectOptions();
       QString dsn= QString("DRIVER={SQL Server};Server=%1;Database=%2;").arg(serverName).arg(dbName);
       db.setDatabaseName(dsn);

        if(db.open())
        {
            qDebug() << ("Connected to database");
            return true;
        }
        else
        {
            qDebug() << ("Failed to open the database");
            return false;
        }
    }

    void closeDBConnection()
    {

        if(db.isValid())
        {
            qDebug() <<"Connection Valid";
            db.close();
        }
        else
        {
            qDebug() <<"Connection Invalid";
        }

    }

    bool checkUserIsExist(QString& login , QString& password)
    {
        if(!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }

        QSqlQuery qry;

        if(qry.exec("SELECT adminLogin, adminPass "
                    "FROM adminLogIn "
                    "WHERE adminLogin = \'" + login + "\'"
                    "AND adminPass = \'" +  password+  "\'"))
        {
            if(qry.next())
            {
                return true;
            }

        }
        return false;
    }

    QSqlQueryModel* getDepartmentList()
    {
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT DISTINCT(department) FROM company");

        if(qry->exec())
        {
            model->setQuery(*qry);
            qDebug() << "Sucesss";
        }
        else
        {
            qDebug() << "Failed";
        }

        return model;
    }

    QSqlQueryModel* getDesignationList(QComboBox* dept)
    {
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT designation FROM company WHERE department='" + dept->currentText() + "';");

        if(qry->exec())
        {
            model->setQuery(*qry);
            qDebug() << "Sucesss";
        }
        else
        {
            qDebug() << "Failed";
        }

        return model;
    }

    QString getLastID()
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT MAX(Employee_ID) FROM employee");
        qry->exec();
        qry->next();

        int lastID = qry->value(0).toInt() + 1;

        return QString::number(lastID);
    }

    QString getTotalEmployee()
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT COUNT(Employee_ID) FROM employee");
        qry->exec();
        qry->next();

        int lastID = qry->value(0).toInt();

        return QString::number(lastID);
    }

    QString getTotalDept()
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT COUNT(DISTINCT(department)) FROM company");
        qry->exec();
        qry->next();

        int lastID = qry->value(0).toInt();

        return QString::number(lastID);
    }

    QString getTotalDesign()
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT COUNT(DISTINCT(designation)) FROM company");
        qry->exec();
        qry->next();

        int lastID = qry->value(0).toInt();

        return QString::number(lastID);
    }

    QString getEmployeeID(QString dept, QString design)
    {
        return dept + "/" + design + "/" + getLastID();
    }

    QString getDesignationShortName(QComboBox* design)
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT design_short FROM company WHERE designation='" + design->currentText() + "';");
        qry->exec();
        qry->next();

        QString x = qry->value(0).toString();

        return x;
    }

    QString getDepartmentShortName(QComboBox* dept)
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT dept_short FROM company WHERE department='" + dept->currentText() + "';");
        qry->exec();
        qry->next();

        QString x = qry->value(0).toString();

        return x;
    }

    int getSalary(QComboBox* dept, QComboBox* design)
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT salary FROM company WHERE department='" + dept->currentText() + "' AND designation='" +design->currentText() + "';");
        qry->exec();
        qry->next();

        int x = qry->value(0).toInt();

        return x;
    }

    QSqlQueryModel* getEmployeeDetails()
    {
        if(db.isOpen())
        {
            QSqlQueryModel *querModel = new QSqlQueryModel();
            querModel->setQuery("SELECT id, name, department, designation, phone, email FROM employee;");
            return querModel;
        }
        //error
        return 0;
    }

    void searchEmployeeDetails(QTableView* tableView, QString searchText)
    {
        if(db.isOpen())
        {
            QSqlQueryModel *querModel = new QSqlQueryModel();
            querModel->setQuery("SELECT id, name, department, designation, phone, email FROM employee WHERE name LIKE '" + searchText + "%' OR id LIKE '" + searchText +"%';");
            tableView->setModel(querModel);
        }
    }

    void setEmployeeUpdateDetails(QTableView* tableView)
    {
        if(db.isOpen())
        {
            QSqlQueryModel *querModel = new QSqlQueryModel();
            querModel->setQuery("SELECT id, name, phone FROM employee;");
            tableView->setModel(querModel);
        }
    }

    QSqlQuery* showEmployeeDetailsToLineEdit(QString id)
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT * FROM employee WHERE id='" + id + "';");
        qry->exec();
        qry->next();

        return qry;
    }

    void deleteEmployeeRecord(QString id)
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("DELETE  FROM employee WHERE id='" + id + "';");

        qry->exec();
    }


};
