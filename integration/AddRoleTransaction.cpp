#include "AddRoleTransaction.h"
#include "memory/TermRole.h"

#include <QStringBuilder>

AddRoleTransaction::AddRoleTransaction(quint32 eid, QString roleName, double roleSalary, Date* startDate, Date* endDate, quint8 roleType)
{
    this->eid = eid;
    this->roleName = roleName;
    this->roleSalary = roleSalary;
    this->startDate = startDate;
    this->endDate = endDate;
    this->roleType = roleType;
}

/**
 * @brief AddRoleTransaction::apply
 * @param em the entity management ref
 * @return true or false for success or fail
 */
bool AddRoleTransaction::apply(EntityManager &em)
{
    EmployeeRole* er;

    if (endDate == NULL) {
        er = new ContinuingRole(0, roleName, roleSalary, startDate, roleType);
    } else {
        er = new TermRole(0, roleName, roleSalary, startDate, endDate, roleType);
    }

    em.addEmployeeRole(eid, er);

    return (true);
}

/**
 * @brief AddRoleTransaction::displayText
 * @return human readable verion of the transaction
 */
QString AddRoleTransaction::displayText()
{
    QString ret = "AddRole - eid: " % QString::number(eid)
            % ", role name: " % roleName
            % ", start date: " % startDate->toString()
            % ", end date: " % (endDate->toInteger() == 0 ? "No end date" : endDate->toString()) % ", ";

    if ((roleType & EmployeeRole::DURATION_MASK) == EmployeeRole::DURATION_CONT) {
        ret = ret % "Continuing ";
    } else {
        ret = ret % "Term ";
    }

    if ((roleType & EmployeeRole::TIME_MASK) == EmployeeRole::TIME_PART) {
        ret = ret % "Part-time ";
    } else {
        ret = ret % "Full-time ";
    }

    if ((roleType & EmployeeRole::TYPE_MASK) == EmployeeRole::TYPE_FACULTY) {
        ret = ret % "Faculty";
    } else if ((roleType & EmployeeRole::TYPE_MASK) == EmployeeRole::TYPE_STAFF) {
        ret = ret % "Staff";
    } else if ((roleType & EmployeeRole::TYPE_MASK) == EmployeeRole::TYPE_TA) {
        ret = ret % "TA";
    } else {
        ret = ret % "RA";
    }

    return (ret);
}
