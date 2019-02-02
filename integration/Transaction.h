#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "memory/EntityManager.h"

/**
 * @brief The Transaction class
 *
 * An inteface for grouping transaction objects. A transaction
 * must be able to be applied to the EntityManager and be printable
 * to screen for reference of what transactions occured.
 *
 * @author Michael van Dyk
 */
class Transaction
{
public:
    Transaction();

    /**
     * @brief apply
     *      Used to apply the transaction to the entity manager. The
     * entity manager will be used to update the database.
     * @param em a reference to the entity manager
     * @return true if successful
     */
    virtual bool     apply(EntityManager &em) = 0;

    /**
     * @brief displayText
     *      Displays the transaction in a human readable way. This is
     * since the file it self is not readable other than some strings
     * that are surrounded by control characters.
     * @return a human readable representation of the transaction.
     */
    virtual QString  displayText()            = 0;
};

#endif // TRANSACTION_H
