#include "ApplyRaiseTransaction.h"
#include "memory/EmployeeRole.h"

#include <QString>
#include <QStringBuilder>

ApplyRaiseTransaction::ApplyRaiseTransaction(Date* applyDate, quint8 roleType, bool percent, double amount)
{
    this->applyDate = applyDate;
    this->roleType = roleType;
    this->percent = percent;
    this->amount = amount;
}

/**
 * @brief ApplyRaiseTransaction::apply
 * @param em the entity management ref
 * @return true or false for success or fail
 */
bool ApplyRaiseTransaction::apply(EntityManager &em)
{
    em.applyRaise(new Raise(roleType, applyDate, percent, amount));
    return (true);
}

/**
 * @brief ApplyRaiseTransaction::displayText
 * @return human readable verion of the transaction
 */
QString ApplyRaiseTransaction::displayText()
{
    QString ret = "Apply Raise - Apply Date: " % applyDate->toString() % ", RoleType: ";

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
        ret = ret % "Faculty, ";
    } else if ((roleType & EmployeeRole::TYPE_MASK) == EmployeeRole::TYPE_STAFF) {
        ret = ret % "Staff, ";
    } else if ((roleType & EmployeeRole::TYPE_MASK) == EmployeeRole::TYPE_TA) {
        ret = ret % "TA, ";
    } else {
        ret = ret % "RA, ";
    }

    ret = ret % (percent ? "percent raise, " : "fixed amount raise, ");
    if (!percent)
        ret = ret % "$" % QString::number(amount);
    else
        ret = ret % QString::number(amount) % "%";

    return (ret);
}
