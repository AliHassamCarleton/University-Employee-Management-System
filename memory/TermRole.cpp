#include "TermRole.h"

TermRole::TermRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, Date* endDate, quint8 type)
    : EmployeeRole::EmployeeRole(roleID, roleName, roleSalary, startDate, type)
{
    this->endDate = endDate;
}

TermRole::TermRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, Date* endDate, quint8 time, quint8 type)
    : EmployeeRole::EmployeeRole(roleID, roleName, roleSalary, startDate, DURATION_TERM, time, type)
{
    this->endDate = endDate;
}

TermRole::~TermRole()
{
    delete endDate;
}


Date& TermRole::getEndDate()
{
    return (*endDate);
}
