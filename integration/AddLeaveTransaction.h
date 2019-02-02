#ifndef ADDLEAVETRANSACTION_H
#define ADDLEAVETRANSACTION_H

#include "Transaction.h"
#include "memory/Date.h"

/**
 * @brief The AddLeaveTransaction class
 *
 * Used to represent a transaction that adds a leave period to cuNICS.
 *
 * @author Michael van Dyk
 */
class AddLeaveTransaction : public Transaction
{
public:
    AddLeaveTransaction(quint32 eid, quint8 role, Date* start, Date* end, double percent);

    virtual bool    apply(EntityManager &em);
    virtual QString displayText();

private:
    quint32 eid;     // The employee id of the employee to apply the leave to
    quint8  role;    // The role index in the employee role array to apply the leave to
    Date*   start;   // The start date of teh leave period
    Date*   end;     // the end date of the leave
    double  percent; // The reduction percentage ofd the leave
};

#endif // ADDLEAVETRANSACTION_H
