#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H
#include <QString>

namespace msg
{

enum header : quint8
{
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
    getEmployeeNameIdSurname,
    //
    getAttandanceDate,
    setEmployeeAttandanceEntered,
    setEmployeeAttandanceExit,
    setAttandanceSuccess,
    setAttandanceFailure,
    // autopilot
    insertInDBSettingPayment,
    insertedInDBSettingStatus,
    getSettingsData,
    getReportPeriodInfo,
    getDetailInfoPaymentPeriod,
    getPdfData,
    getSearchedEmployee
};

struct status
{
    QString success = "success";
    QString failure = "failed";
    QString alreadyAutorized = "already";
    QString unknownError = "error";
    QString empty = "empty";
};
} // namespace msg

#endif // MESSAGEHEADER_H
