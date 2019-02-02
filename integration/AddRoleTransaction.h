#ifndef ADDROLETRANSACTION_H
#define ADDROLETRANSACTION_H

#include "Transaction.h"
#include "memory/Date.h"

/**
 * @brief The AddRoleTransaction class
 *
 * Used to represent a transaction that adds a role to cuNICS.
 *
 * @author Michael van Dyk
 */
class AddRoleTransaction : public Transaction
{
public:
    AddRoleTransaction(quint32 eid, QString roleName, double roleSalary, Date* startDate, Date* endDate, quint8 roleType);

    virtual bool     apply(EntityManager &em);
    virtual QString  displayText();

private:
    quint32 eid;        // the employee id of the employee who is getting the role.
    QString roleName;   // The role name
    double  roleSalary; // The base role salary
    Date*   startDate;  // The starting dateof the role
    Date*   endDate;    // The end date of the role, 0/0/0 if continuing role
    quint8  roleType;   // The role type of the role being added
};

#endif // ADDROLETRANSACTION_H
