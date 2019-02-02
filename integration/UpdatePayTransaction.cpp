#include "UpdatePayTransaction.h"

#include <QStringBuilder>

UpdatePayTransaction::UpdatePayTransaction(quint32 eid, quint8 roleIndex, double amount)
{
    this->eid = eid;
    this->roleIndex = roleIndex;
    this->amount = amount;
}

/**
 * @brief UpdatePayTransaction::apply
 * @param em the entity management ref
 * @return true or false for success or fail
 */
bool UpdatePayTransaction::apply(EntityManager &em)
{
    EmployeeRole* er;

    er = em.getEmployee(eid)->getEmployeeRoles().value(roleIndex);
    er->setSalary(amount);
    em.updateEmployeeRole(er);

    return (true);
}

/**
 * @brief UpdatePayTransaction::displayText
 * @return human readable verion of the transaction
 */
QString UpdatePayTransaction::displayText()
{
    return ("Update Pay - employee id: " % QString::number(eid) % ", role index: " % QString::number((int)roleIndex) % ", $" % QString::number(amount));
}
