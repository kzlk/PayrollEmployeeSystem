#ifndef CPAYMENT_H
#define CPAYMENT_H

#include "dbUtils.cpp"
#include "qdatetime.h"
#include <QRunnable>
#include <QThreadPool>

class CPayment
{
  public:
    CPayment(DatabaseUtils &db);
    DatabaseUtils dbUtils{};
    void doPay();
    // void run() override;

    const QDateTime &getStartPeriodPayment() const;

    void setStartPeriodPayment(const QDateTime &newStartPeriodPayment);

    void setEndPeriodPayment(const QDateTime &newEndPeriodPayment);

  private:
    QDateTime startPeriodPayment = {};
    QDateTime endPeriodPayment = {};
};

#endif // CPAYMENT_H
