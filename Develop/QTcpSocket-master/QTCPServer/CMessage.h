#ifndef CMESSAGE_H
#define CMESSAGE_H
#include <QString>

namespace msg
{
    struct header
    {
        QString autorazation = "autorazation";
    };

    struct status
    {
        QString success = "success";
        QString failure = "failed";
    };
}
#endif // CMESSAGE_H
