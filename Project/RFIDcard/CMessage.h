#ifndef CMESSAGE_H
#define CMESSAGE_H
#include "qglobal.h"
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

} // namespace msg
#endif // CMESSAGE_H
