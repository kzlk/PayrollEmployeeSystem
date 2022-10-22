#ifndef CAUTOPILOT_H
#define CAUTOPILOT_H

#include "cpayment.h"
#include "dbUtils.cpp"
#include <QMainWindow>
#include <QObject>

class CAutoPilot : public QObject
{
    Q_OBJECT
  public:
    CAutoPilot();
    ~CAutoPilot();
    CAutoPilot(quint64 timeShotInMs, DatabaseUtils &db);
    void start();
    void stop();

  private:
    CPayment payment{};
    QTimer *timer{};
    quint64 timeShotInMs{};
    DatabaseUtils dbUtils{};

    QDateTime systemDate{};
    QDateTime nextPaymentDate{};
    QDateTime startPeriodPayment{};
    QDateTime endPeriodPayment{};

    quint8 paymentFrequency{};

    void updatePaymentDate();

    bool isAutoPilotOnAndSetData();

    bool checkDate();

  public slots:
    void timerSlot();
};

#endif // CAUTOPILOT_H
