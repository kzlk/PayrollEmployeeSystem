#ifndef CPAYMENT_H
#define CPAYMENT_H

#include "qdatetime.h"
#include <QRunnable>
#include <QThreadPool>

class CPayment : public QRunnable
{
  public:
    CPayment();
    void doPay(QDateTime start, QDateTime end);
    void run() override;
};

#endif // CPAYMENT_H
