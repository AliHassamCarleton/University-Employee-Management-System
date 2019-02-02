#ifndef UPDATEBASICTRANSACTION_H
#define UPDATEBASICTRANSACTION_H

#include "Transaction.h"
#include "AttributePair.h"

#include <QList>

/**
 * @brief The UpdateBasicTransaction class
 *
 * Used to represent a transaction that upadate basic employee
 * information in cuNICS.
 *
 * @author Michael van Dyk
 */
class UpdateBasicTransaction : public Transaction
{
public:
    UpdateBasicTransaction(quint32 eid, QList<AttributePair*>* attributes);

    virtual bool    apply(EntityManager &em);
    virtual QString displayText();

private:
    quint32                eid;        // the employee id
    QList<AttributePair*>* attributes; // the list of attributes to update
};

#endif // UPDATEBASICTRANSACTION_H
