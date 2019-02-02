#ifndef CONTINUINGROLE_H
#define CONTINUINGROLE_H

#include <QList>

#include "EmployeeRole.h"
#include "LeavePeriod.h"

/**
 * @brief The ContinuingRole class
 * A specialized form of the EmployeeRole. This form allows for
 * the addition of LeavePeriods which are available to continuing employee roles.
 *
 * Attributes:
 *  - leaves : the list of LeavePeriods
 *
 * @author Daniel Homer
 * @author Michael van Dyk
 */
class ContinuingRole : public EmployeeRole
{
public:
    /** Constructor for DatabaseControl */
    ContinuingRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, quint8 type);
    /** Constructor for EntityControl */
    ContinuingRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, quint8 time, quint8 type);
    ~ContinuingRole();

    void addLeavePeriod(LeavePeriod* newLeave);
    QList<LeavePeriod*>& getLeavePeriods();

private:
    QList<LeavePeriod*> leaves;

};

#endif // CONTINUINGROLE_H
