#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H
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
  addEmployee
};

struct status {
  QString success = "success";
  QString failure = "failed";
  QString alreadyAutorized = "already";
  QString unknownError = "error";
};
} // namespace msg

#endif // MESSAGEHEADER_H
