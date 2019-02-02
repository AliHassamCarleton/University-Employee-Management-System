#ifndef UPDATEPAYTRANSACTION_H
#define UPDATEPAYTRANSACTION_H

#include "Transaction.h"

#include <QIntegerForSize>

/**
 * @brief The UpdatePayTransaction class
 *
 * Used to represent a transaction that updates a role's pay info in cuNICS.
 *
 * @author Michael van Dyk
 */
class UpdatePayTransaction : public Transaction
{
public:
    UpdatePayTransaction(quint32 eid, quint8 roleIndex, double amount);

    virtual bool    apply(EntityManager &em);
    virtual QString displayText();

private:
    quint32 eid;       // the employee id
    quint8  roleIndex; // the index of the role int he employee's role list
    double  amount;    // the amount to change the wage to

};

#endif // UPDATEPAYTRANSACTION_H
