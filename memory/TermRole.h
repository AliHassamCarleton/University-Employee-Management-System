#ifndef TERMROLE_H
#define TERMROLE_H

#include "EmployeeRole.h"

/**
 * @brief The TermRole class
 *
 * @author Daniel Homer
 * @author Michael van Dyk
 */
class TermRole : public EmployeeRole
{
public:
    TermRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, Date* endDate, quint8 type);
    TermRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, Date* endDate, quint8 time, quint8 type);
    ~TermRole();

    Date& getEndDate();

private:
    Date* endDate;

};

#endif // TERMROLE_H
