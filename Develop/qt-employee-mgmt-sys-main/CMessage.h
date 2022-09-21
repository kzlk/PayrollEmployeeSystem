#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H
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
};

#endif // MESSAGEHEADER_H


