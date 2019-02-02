#include "AddLeaveTransaction.h"

#include "memory/ContinuingRole.h"

#include <QString>
#include <QStringBuilder>

#include <QDebug>

AddLeaveTransaction::AddLeaveTransaction(quint32 eid, quint8 role, Date* start, Date* end, double percent)
{
    this->eid = eid;
    this->role = role;
    this->start = start;
    this->end = end;
    this->percent = percent;
}

/**
 * @brief AddLeaveTransaction::apply
 * @param em the entity management ref
 * @return true or false for success or fail
 */
bool AddLeaveTransaction::apply(EntityManager &em)
{
    em.addLeavePeriod((ContinuingRole*)em.getEmployee(eid)->getEmployeeRoles().value(role), new LeavePeriod(0, start, end, percent));
    return (true);
}

/**
 * @brief AddLeaveTransaction::displayText
 * @return  human readable verion of the transaction
 */
QString AddLeaveTransaction::displayText()
{
    return ("AddLeave - eid: " % QString::number(eid)
            % ", role index: " % QString::number((int)role)
            % ", start date: " % start->toString()
            % ", end date: " % end->toString()
            % ", percent of salary: " % QString::number(percent) % "%");
}
