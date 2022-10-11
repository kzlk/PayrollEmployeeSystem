#ifndef CMESSAGE_H
#define CMESSAGE_H
#include <QString>

namespace msg
{
    struct header
    {
        QString autorazation = "autorazation";
        QString employeeDetail     = "employeeDetail";
        QString totalEmployee = "totalEmployee";
        QString totalDepartment = "totalDept";
        QString totalDesign =     "totalDesign";
        QString totalInfoEmployee = "totalInfoEmployee";
        QString fillInfoCompany =    "infoCompany";
        QString infoDemp =    "infoDepartment";

    };

    struct status
    {
        QString success = "success";
        QString failure = "failed";
    };
}
#endif // CMESSAGE_H
