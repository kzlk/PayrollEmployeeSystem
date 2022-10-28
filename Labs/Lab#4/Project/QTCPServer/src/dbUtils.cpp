#include "qdatetime.h"
#include <QComboBox>
#include <QDebug>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QTableView>
#include <QThread>
#include <QtSql/QSqlDatabase>

class DatabaseUtils
{
  public:
    // connection to MS SQL SERVER database
    QString serverName = "localhost\\sqlexpress";
    QString dbName = "payrolldb";
    QSqlDatabase db{};

    bool connectToDB()
    {

        db = QSqlDatabase::addDatabase("QODBC");

        db.setConnectOptions();
        QString dsn = QString("DRIVER={SQL Server};Server=%1;Database=%2;")
                          .arg(serverName)
                          .arg(dbName);
        db.setDatabaseName(dsn);

        try
        {
            if (!db.open())
                throw "Failed to open the database";
            else
            {
                qDebug() << ("Connected to database");
                return true;
            }
        }
        catch (QString &myExeption)
        {
            qDebug() << myExeption;
            return false;
        };
    }

    void dropTableAuthUser()
    {
        QSqlQuery qry;
        qry.exec("DELETE FROM  dbo.AutorizationUser");
    }

    /*AutoPilot*/

    bool checkIfAutoPilotIsOn()
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }

        QSqlQuery qry;
        if (qry.exec("SELECT [AutoPilot] FROM [dbo].[SystemSettings]"))
        {
            if (qry.next())
            {
                if (qry.value(0).toInt() == 1)
                {
                    return true;
                }
            }
        }
        return false;
    }

    QSqlQueryModel *getDataFromSystemSetting()
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQueryModel *querModel = new QSqlQueryModel();
        querModel->setQuery("SELECT [StartPeriodPayment], "
                            "[NextPayment], "
                            "[LastPayment] ,"
                            "[Frequency], "
                            "[Configuration], "
                            "[payDate],"
                            "[firstPayDateOnNewSchedule] "
                            " FROM [dbo].[SystemSettings]");

        return querModel;
    }

    QSqlQueryModel *getDataDetailPaymentTable(int &id)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQueryModel *querModel = new QSqlQueryModel();
        querModel->setQuery(
            QString(
                "SELECT employee.ID , CAST(employee.Father as varchar) + ', ' "
                "+ CAST(employee.Name as varchar), "
                "PaymentInfoDetail.total_hours, "
                "PaymentInfoDetail.gross_pay, "
                "PaymentInfoDetail.taxes, "
                "PaymentInfoDetail.net_pay "
                "FROM PaymentInfoDetail  "
                "JOIN PaymentPeriod "
                "ON PaymentInfoDetail.payment_period_id =   "
                "PaymentPeriod.payment_id "
                "JOIN employee "
                "ON PaymentInfoDetail.employee_id = employee.ID "
                "WHERE payment_period_id = %1 ")
                .arg(id));

        return querModel;
    }

    QSqlQueryModel *getPaymentDataForPDFGenerate(QString &emp_id, int &pay_id)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQueryModel *querModel = new QSqlQueryModel();
        querModel->setQuery(
            QString("SELECT *FROM getDataForPdfReportPayment ('%1', %2) ")
                .arg(emp_id)
                .arg(pay_id));

        return querModel;
    }

    QSqlQueryModel *getPaymentPeriodData()
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQueryModel *querModel = new QSqlQueryModel();
        querModel->setQuery("SELECT *FROM PaymentPeriod");

        return querModel;
    }

    bool setConfigurationToUnActive()
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQuery qry;
        if (qry.exec(" UPDATE dbo.SystemSettings SET Configuration = 0"))
        {
            return true;
        }
        return false;
    };

    bool getConfigurationStatus()
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }
        QSqlQuery qry;
        if (qry.exec("SELECT Configuration FROM dbo.SystemSettings"))
        {
            if (qry.next())
            {
                if (qry.value(0).toInt() == 1)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool insertPaymentPeriod()
    {
        return false;
    }

    qint8 getPaymentPeriodID(QDateTime &start, QDateTime &end)
    {
    }

    bool doPay(QDateTime &start, QDateTime &end)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }
        qDebug() << "Hello from Do Pay";
        /*EXECUTE STORED PROCEDURE*/
        QSqlQuery query;
        if (!query.prepare("{CALL payrolldb.dbo.storeDataToPaymentPeriod "
                           "(:start, :end)} "))
        {
            qDebug() << "Error in prepare  stored function";
            return false;
        }
        else
        {
            query.bindValue(":start", start);
            query.bindValue(":end", end);
            if (!query.exec())
            {
                return false;
                qDebug() << "Error in exec  stored function!";
            }
            qDebug() << "Successful done stored procedure!";
            return true;
        }
        return false;
    }

    bool insertDataInfoPayment()
    {
        return false;
    }

    bool updateSystemSetting(QDateTime startPeriodPayment,
                             QDateTime endPeriodPayment, QDateTime nextPayment,
                             QDateTime payDate)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQuery qry;

        if (!qry.prepare(QString("UPDATE dbo.SystemSettings "
                                 " SET LastPayment = :lastPeriod , "
                                 " StartPeriodPayment = :startPeriod , "
                                 " NextPayment = :nextPeriod , "
                                 " payDate = :payDate ")))
        {
            return false;
        }
        else
        {
            qry.bindValue(":lastPeriod", endPeriodPayment);
            qry.bindValue(":startPeriod", startPeriodPayment);
            qry.bindValue(":nextPeriod", nextPayment);
            qry.bindValue(":payDate", payDate);

            if (!qry.exec())
            {
                return false;
            }

            return true;
        }

        return false;
    }

    void closeDBConnection()
    {
        if (db.isValid())
        {
            qDebug() << "Connection Valid";
            db.close();
        }
        else
        {
            qDebug() << "Connection Invalid";
        }
    }

    bool dropUser(const QString &login)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }

        QSqlQuery qry;
        if (qry.exec(QString("DELETE FROM dbo.AutorizationUser"
                             " WHERE Admin_id = '%1'")
                         .arg(login)))
        {
            qDebug() << "The " << login << "Was deleted";
            return true;
        }
        return false;
    }

    bool checkUniqueId(const quint64 &uniqueId)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }
        QSqlQuery qry;
        if (qry.exec(QString("SELECT Unique_user_id FROM dbo.AutorizationUser "
                             "WHERE Unique_user_id = %1 ")
                         .arg(uniqueId)))
        {
            if (qry.next())
            {
                return true;
            }
        }

        return false;
    }

    QSqlQueryModel *checkIfEmloyeeNotExit(QString &empId)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQueryModel *querModel = new QSqlQueryModel();
        querModel->setQuery(
            QString("SELECT EnterDate FROM "
                    "dbo.Employee_Attandance "
                    "WHERE LeftDate IS NULL AND Employee_ID  = '%1'")
                .arg(empId));
        return querModel;
    }

    bool insertEnteredAttandance(QDateTime &enterDate, QString &empId)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQuery qry;

        if (!qry.prepare(
                QString("INSERT INTO dbo.Employee_Attandance(Employee_ID, "
                        "EnterDate) VALUES "
                        " (:id, :dateTime)")))
        {
            return false;
        }
        else
        {
            qry.bindValue(":id", empId);
            qry.bindValue(":dateTime", enterDate);
            if (!qry.exec())
            {
                return false;
            }

            return true;
        }

        return false;
    }

    bool updateExitAttandance(QDateTime &enterDate, QString &empId)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQuery qry;

        if (!qry.prepare(QString("UPDATE  dbo.Employee_Attandance "
                                 "SET LeftDate = :dateTime "
                                 "WHERE LeftDate IS NULL "
                                 "AND Employee_ID = :id ")
                             .arg(enterDate.toString(), empId)))
        {
            return false;
        }
        else
        {
            qry.bindValue(":id", empId);
            qry.bindValue(":dateTime", enterDate);
            if (!qry.exec())
            {
                return false;
            }

            return true;
        }

        return false;
    }

    bool insertIntoSettingTable(QDateTime &startPeriod, QDateTime endPeriod,
                                QDateTime &nextPayment, qint8 autopilot,
                                qint8 frequency)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQuery qry;
        QSqlQuery qry1;
        if (qry1.exec("TRUNCATE TABLE SystemSettings"))
        {
            if (!qry.prepare(
                    QString("INSERT INTO SystemSettings "
                            "(StartPeriodPayment, NextPayment, LastPayment, "
                            "AutoPilot,Frequency, Configuration, "
                            "firstPayDateOnNewSchedule, payDate) "
                            "VALUES (:start, :next, :end, :autop, :freq, "
                            ":config, :newShedule, :payDate)")))
            {
                return false;
            }
            else
            {
                qry.bindValue(":start", startPeriod);
                qry.bindValue(":next", nextPayment);
                qry.bindValue(":autop", autopilot);
                qry.bindValue(":freq", frequency);
                qry.bindValue(":end", endPeriod.addDays(-1));
                qry.bindValue(":config", 1);
                qry.bindValue(":newShedule", endPeriod);
                qry.bindValue(":payDate", endPeriod);

                if (!qry.exec())
                {
                    return false;
                }

                return true;
            }
        }

        return false;
    }

    QSqlQueryModel *getAttandanceInfo(QString &employeeId)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQueryModel *querModel = new QSqlQueryModel();
        querModel->setQuery(QString("SELECT Cast(EnterDate as smalldatetime) "
                                    "FROM dbo.Employee_Attandance "
                                    "WHERE LeftDate IS NULL "
                                    "AND Employee_ID = '%1' ")
                                .arg(employeeId));
        return querModel;
    }

    QSqlQueryModel *getNameIdSurname()
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return 0;
        }

        QSqlQueryModel *querModel = new QSqlQueryModel();
        querModel->setQuery("SELECT Name, Father, ID FROM dbo.employee");
        return querModel;
    }

    bool insertUserToAutorizedTable(const QString &login,
                                    const quint64 &uniqueId,
                                    const qint32 &socketDescriptor)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }

        QSqlQuery qry;

        if (qry.exec(
                QString(
                    "INSERT INTO dbo.AutorizationUser(Admin_id, "
                    "Unique_user_id, Socket_descriptor) VALUES ('%1', %2 , %3)")
                    .arg(login)
                    .arg(uniqueId)
                    .arg(socketDescriptor)))
        {
            return true;
        }

        return false;
    };

    bool addEmployee(QVariantList &employee)
    {

        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }

        QSqlQuery *sqlQuery = new QSqlQuery(db);
        if (db.isOpen())
        {
            sqlQuery->prepare(
                "insert into Employee (ID, Name, DOB, Gender, Father, Email, "
                "Phone, "
                "Address, Department, Designation, DOJ, Salary, Type) "
                "values ('" +
                employee.at(0).toString() + "' ,'" + employee.at(1).toString() +
                "' , '" + employee.at(2).toString() + "' , '" +
                employee.at(3).toString() + "' , '" +
                employee.at(4).toString() + "' , '" +
                employee.at(5).toString() + "' , '" +
                employee.at(6).toString() + "' , '" +
                employee.at(7).toString() + "' , '" +
                employee.at(8).toString() + "' , '" +
                employee.at(9).toString() + "' , '" +
                employee.at(10).toString() + "' , " +
                employee.at(11).toString() + ", '" +
                employee.at(12).toString() + "');");

            if (sqlQuery->exec())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        return false;
    }

    bool checkUserIsAutorized(QString &login)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }

        QSqlQuery qry;
        auto a = QString("SELECT dbo.AutorizationUser.Admin_id"
                         " FROM dbo.adminLogIn INNER JOIN"
                         " dbo.AutorizationUser ON dbo.adminLogIn.adminLogin = "
                         " dbo.AutorizationUser.Admin_id"
                         " WHERE dbo.adminLogIn.adminLogin = '%1' ")
                     .arg(login);
        qDebug() << a;
        if (qry.exec(a))
        {
            qDebug() << "True " << qry.value(0);
            if (qry.next())
            {
                return true;
            }
        }

        return false;
    };

    bool checkUser(QString &login, QString &password)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }

        QSqlQuery qry;

        qry.exec(QString("DECLARE @resp bit "
                         "EXEC dbo.uspLogin N'%1', N'%2', @resp OUTPUT "
                         "SELECT	 @resp as N'@responseMessage'")
                     .arg(login)
                     .arg(password));

        qry.next();

        return qry.value(0).toBool();
    }

    bool checkUserIsExist(QString &login, QString &password)
    {
        if (!db.isOpen())
        {
            qDebug() << "No connection to db :( ";
            return false;
        }

        QSqlQuery qry;

        if (qry.exec("SELECT adminLogin, adminPass "
                     "FROM adminLogIn "
                     "WHERE adminLogin = \'" +
                     login +
                     "\'"
                     "AND adminPass = \'" +
                     password + "\'"))
        {
            if (qry.next())
            {
                return true;
            }
        }
        return false;
    }

    QSqlQueryModel *getDepartmentList()
    {
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT DISTINCT(department) FROM company");

        if (qry->exec())
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

    QSqlQueryModel *getDesignationList(QString &dept)
    {
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT designation FROM company WHERE department = '" +
                     dept + "';");

        if (qry->exec())
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

    QString getDesignationShortName(QString &design)
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT design_short FROM company WHERE designation='" +
                     design + "';");
        qry->exec();
        qry->next();

        QString x = qry->value(0).toString();

        return x;
    }

    QString getDepartmentShortName(QString &dept)
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT dept_short FROM company WHERE department='" +
                     dept + "';");
        qry->exec();
        qry->next();

        QString x = qry->value(0).toString();

        return x;
    }

    int getSalary(QString &dept, QString &design)
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("SELECT salary FROM company WHERE department='" + dept +
                     "' AND designation='" + design + "';");
        qry->exec();
        qry->next();

        int x = qry->value(0).toInt();

        return x;
    }

    QSqlQueryModel *getEmployeeDetails()
    {
        if (db.isOpen())
        {
            QSqlQueryModel *querModel = new QSqlQueryModel();
            querModel->setQuery(
                "SELECT id, name, department, designation, phone, "
                "email FROM employee;");
            return querModel;
        }
        // error
        return 0;
    }

    QSqlQueryModel *searchEmployeeDetails(QString searchText)
    {
        if (db.isOpen())
        {
            QSqlQueryModel *querModel = new QSqlQueryModel();
            querModel->setQuery(
                "SELECT id, name, department, designation, phone, "
                "email FROM employee WHERE name LIKE '" +
                searchText + "%' OR id LIKE '" + searchText + "%';");
            return querModel;
        }

        return 0;
    }

    QSqlQueryModel *setEmployeeUpdateDetails()
    {
        if (db.isOpen())
        {
            QSqlQueryModel *querModel = new QSqlQueryModel();
            querModel->setQuery("SELECT id, name, phone FROM employee;");
            return querModel;
        }
        return 0;
    }

    QSqlQueryModel *showEmployeeDetailsToLineEdit(QString id)
    {
        if (db.isOpen())
        {

            QSqlQueryModel *querModel = new QSqlQueryModel();
            querModel->setQuery("SELECT employee.Name, "
                                "employee.Father, "
                                "employee.Gender, "
                                "employee.DOB, "
                                "employee.Phone, "
                                "employee.Email, "
                                "employee.Address "
                                "FROM dbo.employee "
                                "INNER JOIN dbo.CompanySettings ON "
                                "employee.company_id = CompanySettings.ID "
                                "WHERE employee.ID = '" +
                                id + "';");
            return querModel;
        }
        return 0;
    }

    QSqlQueryModel *getData(QString id)
    {
        QSqlQueryModel *querModel = new QSqlQueryModel();
        querModel->setQuery("SELECT * FROM employee WHERE id='" + id + "';");
        return querModel;
    }

    bool deleteEmployeeRecord(QString id)
    {
        QSqlQuery *qry = new QSqlQuery(db);

        qry->prepare("DELETE  FROM employee WHERE id='" + id + "';");

        return qry->exec();
    }

    bool setUpdate(QVariantList list)
    {
        QSqlQuery sqlQuery;
        if (db.isOpen())
        {
            sqlQuery.prepare("UPDATE employee SET "
                             "name='" +
                             list.at(0).toString() + "'," + "father='" +
                             list.at(1).toString() + "'," + "email='" +
                             list.at(2).toString() + "'," + "phone='" +
                             list.at(3).toString() + "'," + "address='" +
                             list.at(4).toString() + "'," + "gender='" +
                             list.at(5).toString() + "'," + "dob='" +
                             list.at(6).toString() +
                             "' "
                             " WHERE id = '" +
                             list.at(7).toString() + "';");

            if (sqlQuery.exec())
            {
                return true;
            }
        };

        return false;
    }
};
