#include "cautopilot.h"
#include "qsqlrecord.h"

#include <QTimer>

CAutoPilot::CAutoPilot()
{
}

CAutoPilot::~CAutoPilot()
{
    delete timer;
    delete payment;
}

CAutoPilot::CAutoPilot(quint64 timeShotInMs, DatabaseUtils &db)
{
    this->timeShotInMs = timeShotInMs;

    this->timer = new QTimer(this);

    this->dbUtils = db;

    payment = new CPayment(db);

    connect(timer, &QTimer::timeout, this, &CAutoPilot::timerSlot);
}

void CAutoPilot::start()
{
    if (isAutoPilotOnAndSetData())
    {
        timer->start(timeShotInMs);
    }
}

void CAutoPilot::stop()
{
    timer->stop();
}

void CAutoPilot::updatePaymentDate()
{
    startPeriodPayment = endPeriodPayment;
    endPeriodPayment = endPeriodPayment.addDays(paymentFrequency);
    nextPaymentDate = endPeriodPayment.addDays(paymentFrequency);
}

bool CAutoPilot::isAutoPilotOnAndSetData()
{
    if (dbUtils.checkIfAutoPilotIsOn())
    {
        qDebug() << "Autopilot is : " << dbUtils.checkIfAutoPilotIsOn();

        auto dataFromSysTable = dbUtils.getDataFromSystemSetting();

        startPeriodPayment = dataFromSysTable->record(0).value(0).toDateTime();

        payment->setStartPeriodPayment(startPeriodPayment);

        qDebug() << "Autopilot startDate: " << startPeriodPayment;

        endPeriodPayment = dataFromSysTable->record(0).value(2).toDateTime();

        payment->setEndPeriodPayment(endPeriodPayment);

        qDebug() << "Autopilot endDate : " << endPeriodPayment;

        nextPaymentDate = dataFromSysTable->record(0).value(1).toDateTime();

        paymentFrequency = dataFromSysTable->record(0).value(3).toInt();

        if (dbUtils.setConfigurationToUnActive())
        {
            qDebug() << "Configuration in SystemTable changed to UnActive ";

            return true;
        }
        else
        {
            qDebug() << "Error! Configuration in SystemTable could noy changed "
                        "to UnActive ";
        }
    }
    else
    {
        qDebug() << "AutoPilot is Off from start func";
    }

    return false;
}

bool CAutoPilot::checkDate()
{
    qDebug() << "Current Time = " << QDateTime::currentDateTime();
    if (QDateTime::currentDateTime() >= endPeriodPayment)
        return true;
    return false;
}

void CAutoPilot::timerSlot()
{
    static int count = 0;
    qDebug() << "Hello From TimerSlot  " << count++;

    // checkConfiguration
    // if 1 зчитати дані заново і замінити на 0
    //інакше

    if (!dbUtils.getConfigurationStatus())
    {
        if (checkDate())
        {
            // do pay in new thread
            // add to paremetr databaseUtils
            // set start and end date
            payment->doPay();
            // QThreadPool::globalInstance()->start(payment);

            // calculateNextPayment
            updatePaymentDate();
            // update systemTableSetting
            dbUtils.updateSystemSetting(startPeriodPayment, endPeriodPayment,
                                        nextPaymentDate);
        }
        else
        {
            return;
        }
    }
    // if configuration changed
    else
    {
        if (dbUtils.setConfigurationToUnActive())
        {
            qDebug() << "Configuration in SystemTable changed to UnActive ";

            if (!isAutoPilotOnAndSetData())
                this->stop();
        }
        else
        {
            qDebug() << "Error! Configuration in SystemTable could noy changed "
                        "to UnActive ";
        }
    }
}
