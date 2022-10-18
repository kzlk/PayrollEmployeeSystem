#ifndef CMESSAGE_H
#define CMESSAGE_H
#include <QString>

namespace msg {

enum header : quint8 {
  autorazation = 1,
  employeeDetail,
  totalEmployee,
  totalDepartment,
  totalDesign,
  totalInfoEmployee,
  fillInfoCompany,
  infoDemp,
  infoDesig,
  unAutorizate,
  disconnect,
  salary,
  id,
  addEmployee,
  getEmployeeNameIdSurname
};

//    struct header
//    {
//        QString autorazation = "autorazation";
//        QString employeeDetail     = "employeeDetail";
//        QString totalEmployee = "totalEmployee";
//        QString totalDepartment = "totalDept";
//        QString totalDesign =     "totalDesign";
//        QString totalInfoEmployee = "totalInfoEmployee";
//        QString fillInfoCompany =    "infoCompany";
//        QString infoDemp =    "infoDepartment";

//    };

struct status {
  QString success = "success";
  QString failure = "failed";
  QString alreadyAutorized = "already";
  QString unknownError = "error";
};
} // namespace msg
#endif // CMESSAGE_H
