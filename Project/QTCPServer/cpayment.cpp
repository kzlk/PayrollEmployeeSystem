#include "cpayment.h"
#include "qeventloop.h"

CPayment::CPayment()
{
}

void CPayment::run()
{
    // this->doPay();
    QEventLoop loop;
    qDebug() << "Hello from Thread " << QThread::currentThreadId();
    loop.exec();
}
