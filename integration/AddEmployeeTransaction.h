#ifndef ADDEMPLOYEETRANSACTION_H
#define ADDEMPLOYEETRANSACTION_H

#include "Transaction.h"
#include "memory/Address.h"

#include <QIntegerForSize>

/**
 * @brief The AddEmployeeTransaction class
 *
 * Used to represent a transaction that adds an employee to cuNICS.
 *
 * @author Michael van Dyk
 */
class AddEmployeeTransaction : public Transaction
{
public:
    AddEmployeeTransaction(quint32 eid, QString uid, QString givenName, QString familyName, Address* address);

    virtual bool    apply(EntityManager &em);
    virtual QString displayText();

private:
    quint32  eid;        // The employee id of the new employee
    QString  uid;        // The username of the employee
    QString  givenName;  // the given name of teh employee
    QString  familyName; // The family name of the employee
    Address* address;    // The address of the new employee
};

#endif // ADDEMPLOYEETRANSACTION_H
