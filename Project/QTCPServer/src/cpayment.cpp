#include "cpayment.h"
#include "qeventloop.h"

CPayment::CPayment(DatabaseUtils &db)
{
    dbUtils = db;
}

void CPayment::doPay()
{

    qDebug() << "Hello from doPay " << QThread::currentThreadId()
             << startPeriodPayment.isValid(),
        endPeriodPayment.isValid();
    if (startPeriodPayment.isValid() && endPeriodPayment.isValid())
        dbUtils.doPay(startPeriodPayment, endPeriodPayment);
}

// void CPayment::run()
//{
//     // this->doPay();
//     QEventLoop loop;
//     qDebug() << "Hello from Thread " << QThread::currentThreadId();
//     doPay();
//     loop.exec();
// }

void CPayment::setStartPeriodPayment(const QDateTime &newStartPeriodPayment)
{
    startPeriodPayment = newStartPeriodPayment;
}

void CPayment::setEndPeriodPayment(const QDateTime &newEndPeriodPayment)
{
    endPeriodPayment = newEndPeriodPayment;
}
