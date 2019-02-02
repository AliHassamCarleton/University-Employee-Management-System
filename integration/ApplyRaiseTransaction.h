#ifndef APPLYRAISETRANSACTION_H
#define APPLYRAISETRANSACTION_H

#include "Transaction.h"
#include "memory/Date.h"

/**
 * @brief The ApplyRaiseTransaction class
 *
 * Used to represent a transaction that applies a to cuNICS.
 *
 * @author Michael van Dyk
 */
class ApplyRaiseTransaction : public Transaction
{
public:
    ApplyRaiseTransaction(Date* applyDate, quint8 roleType, bool percent, double amount);

    virtual bool    apply(EntityManager &em);
    virtual QString displayText();

private:
    Date*  applyDate; // The date to apply the raise
    quint8 roleType;  // The type of roles to apply the raise
    bool   percent;   // if the role is percentage based or fixed based
    double amount;    // The amount of raise to perform
};

#endif // APPLYRAISETRANSACTION_H
